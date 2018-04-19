/*
 * csv2db.cpp
 *
 *  Created on: Nov 13, 2013
 *      Author: wladt
 */

#include "csv2db.hpp"
#include <cwt/vector.hpp>
#include <cwt/logger.hpp>
#include <cwt/safeformat.hpp>

/* JSON file format:
 * {
 *    "separator"      : ","             // separator, any char
 *  , "quote"          : "\""            // quote char, any char
 *  , "escape"         : "\\"            // escape char, any char
 *  , "trim_lead_ws"   : "false"         // ignore leading whitespaces, "yes" | "true" | "1" | 1 | "no" | "false" | "0" | 0
 *  , "trim_trail_ws"  : "false"         // ignore trailing whitespaces, see "trim_lead_ws" for values
 *  , "encoding_in"    : "utf8"          // not supported yet, utf8 is default
 *  , "encoding_out"   : "utf8"          // not supported yet, utf8 is default
 *  , "skip_lines"     : 1               // skip lines in CSV file before read records
 *  , "endline"        : "\\n"           // end lines, may be an array
 *  , "columns_num"    : 4               // expected number of columns in the source
 *  , "map"            : [               // mapping CSV fields into SQL fields
 *
 *        {}                             // ignore column (no mapping)
 *  	, { "attr" : "code", "type": "string" }
 * 		, { "attr" : "name", "type": "string" }
 * 		, { "attr" : "currency", "type": "string" }
 *
 *    ]
 *  }
 *
 * Attributes for 'map' entry:
 *	     "type" : <valid cwt::UniType type string representation,
 *                           see cwt::UniType::typeToString() excepting "null">
 *                          ; "bool" / "boolean" / "int" / "integer"
 *                          ; "string" / "float" / "double" / "blob"
 *                          ; "time" / "date" / "datetime"
 *
 *       "pk"        : true | false, default is false
 *       "index"     : true | false, default is false
 *       "nullable"  : true | false, default is false
*        "unique"    : true | false, default is false
 *       "unsigned"  : true | false, default is false
 *       "timestamp" : true | false, default is false
 *       "size"      : unsigned integer, default is 0
 *       "autoinc"   : unsigned integer, default is 0
 *       "default"   : <valid JSON attribute value>, default is undefined.
 */


static bool __init_table(cwt::debby::Table & table, const cwt::JsonValue::array_type & mapping)
{
	size_t size = mapping.size();

	for (size_t i = 0; i < size; ++i) {
		if (!mapping[i]->isObject()) {
			cwt::Logger::error("array 'map' element must be an object");
			return false;
		}

		cwt::JsonValue & spec = *mapping[i];

		// skip column
		if (spec.size() == 0 || spec["attr"].isInvalid())
			continue;

		cwt::String fname = spec["attr"].string();
		cwt::UniType::Type ftype = cwt::UniType::typeFromString(spec["type"].string());
		cwt::debby::Attribute & attr = table.newAttribute(fname, ftype);

		attr.setPk        (spec["pk"].boolean(false));
		attr.setIndexable (spec["index"].boolean(false));
		attr.setNullable  (spec["nullable"].boolean(false));
		attr.setUnique    (spec["unique"].boolean(false));
		attr.setUnsigned  (spec["unsigned"].boolean(false));
		attr.setTimestamp (spec["timestamp"].boolean(false));
		attr.setAutoinc   (spec["autoinc"].integer(0));
		attr.setSize      (size_t(spec["size"].integer(0)));
		attr.setDefault   (spec["default"].string());
	}

	return true;
}


bool Csv2DbContext::convert (const cwt::Json & policy
		, cwt::CsvReader & csvreader
		, cwt::debby::Schema & schema
		, const cwt::String & tname)
{
	if (schema.contains(tname)) {
		cwt::Logger::error(_Fr("%s: table already exists") % tname);
		return false;
	}

	cwt::String separator = policy["separator"].string(cwt::String(1, cwt::Csv::DEFAULT_SEPARATOR));
	cwt::String quote     = policy["quote"].string(cwt::String(1, cwt::Csv::DEFAULT_QUOTE_CHAR));
	cwt::String escape    = policy["escape"].string(cwt::String(1, cwt::Csv::DEFAULT_ESCAPE_CHAR));

	csvreader.setSeparator (separator.isEmpty() ? cwt::Csv::DEFAULT_SEPARATOR   : separator[0]);
	csvreader.setQuote     (quote.isEmpty()     ? cwt::Csv::DEFAULT_QUOTE_CHAR  : quote[0]);
	csvreader.setEscape    (escape.isEmpty()    ? cwt::Csv::DEFAULT_ESCAPE_CHAR : escape[0]);

	csvreader.setIgnoreLeadingWS(policy["trim_lead_ws"].boolean(false));
	csvreader.setIgnoreTrailingWS(policy["trim_trail_ws"].boolean(false));

	if (policy["endline"].isArray()) {
		cwt::Vector<cwt::String> endls;
		cwt::JsonValue::array_type a = policy["endline"].array();

		for (size_t i = 0; i < a.size(); ++i ) {
			cwt::String endl = a[i]->string();
			if (!endl.isEmpty())
				endls.append(a[i]->string());
		}

		csvreader.useEndLines(endls);
	} else {
		csvreader.useEndLine(policy["endline"].string(cwt::Csv::DEFAULT_ENDL));
	}

	long_t skipLines = policy["skip_lines"].integer(0);

	if (skipLines < 0 || skipLines > CWT_INT_MAX) {
		cwt::Logger::error(_Tr("Bad value for 'skip_lines'"));
		return false;
	}

	csvreader.skipLines(int(skipLines));

	size_t line = size_t(skipLines);
	size_t ncolumns = size_t(policy["columns_num"].integer(0));

	if (!policy["map"].isArray()) {
		cwt::Logger::error(_Tr("'map' entry not found or it must be an array"));
		return false;
	}

	cwt::JsonValue::array_type mapping = policy["map"].array();

	if (mapping.size() == 0) {
		cwt::Logger::error(_Tr("'map' entry is empty"));
		return false;
	}

	cwt::debby::Table & table = schema.newTable(tname);

	if (!__init_table(table, mapping))
		return false;

	if (!schema.save())
		return false;

	size_t nfields = mapping.size(); // actual number of fields

	schema.begin();

	cwt::Vector<cwt::String> csvrecord;

	while (!(csvrecord = csvreader.readRecord()).isEmpty()) {
		++line;
		if (csvrecord.size() != ncolumns) {
			cwt::Logger::error(_Fr("Incomplete number of columns at line %u, expected %u")
					% line
					% ncolumns);
			return false;
		}

		cwt::debby::Record record(table);

		for (size_t i = 0; i < ncolumns && i < nfields; ++i) {
			if (mapping[i]->size() > 0) {
				cwt::String attrName = (*mapping[i])["attr"].string(); // not null, already checked by 'init_table'
				record[attrName] = csvrecord[i];
			}
		}

		if (!record.create()) { // insert new record
			schema.rollback();
			return false;
		}

	}
	schema.commit();

	return true;
}



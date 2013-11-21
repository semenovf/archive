/*
 * proc.cpp
 *
 *  Created on: Aug 26, 2013
 *      Author: wladt
 */

#include "proc.hpp"
#include "csv2db.hpp"
#include <cwt/logger.hpp>
#include <cwt/safeformat.hpp>
#include <cwt/io/file.hpp>
#include <cwt/debby.hpp>

int Csv2DbProc::proc()
{
	cwt::App * app = cwt::App::app();
	CWT_ASSERT(app);
	const cwt::Settings & settings = app->settings();

	if (_sourcePath.isEmpty()) {
		cwt::Logger::error("No source specified");
		return -1;
	}

	cwt::String pathToPolicy(settings.string("/policy"));

	if (_sourcePath.isEmpty()) {
		cwt::Logger::error("No policy specified");
		return -1;
	}

	cwt::String uri(settings.string("/db-uri"));

	if (_sourcePath.isEmpty()) {
		cwt::Logger::error("No database URI specified");
		return -1;
	}

	cwt::String tableName(settings.string("/table"));

	if (tableName.isEmpty()) {
		cwt::Logger::error("No table name specified");
		return -1;
	}

	cwt::io::File fileCsv  (_sourcePath, cwt::io::Device::ReadOnly);
	cwt::io::File fileJson (pathToPolicy, cwt::io::Device::ReadOnly);
	io::TextReader readerCsv(fileCsv);
	io::TextReader readerJson(fileJson);

	if (!readerCsv.device()->opened())
		return -1;

	if (!readerJson.device()->opened())
		return -1;

	cwt::debby::Schema schema;
	if (schema.load(uri)) {
		cwt::Logger::error(_Tr("Failed to load schema"));
		return -1;
	}

	CsvReader csvreader(readerCsv, UChar(','));
	Json policy;

	if (!policy.parse(readerJson.readAll()))
		return -1;

	Csv2DbContext converter;
	return converter.convert(policy, csvreader, schema, tableName) ? 0 : -1;
}



/*
 * schema.cpp
 *
 *  Created on: Sep 10, 2013
 *      Author: wladt
 */

//#include "../include/cwt/debby/record.hpp"
#include "../include/cwt/debby/schema.hpp"
#include <cwt/logger.hpp>
#include <cwt/safeformat.hpp>

CWT_NS_BEGIN

namespace debby
{

Table & Schema::newTable (const cwt::String & tname)
{
	CWT_ASSERT(!_schema.contains(tname));
	TablePtr table(new Table());
	table->_pschema = this;
	TableMap::iterator it = _schema.insert(tname, table);
	return *it->second;
}

const Table & Schema::operator [] (const cwt::String & name) const
{
	TableMap::const_iterator it = _schema.find(name);
	CWT_ASSERT(it != _schema.cend());
	return *it->second;
}

Table & Schema::operator [] (const cwt::String & name)
{
	TableMap::iterator it = _schema.find(name);
	CWT_ASSERT(it != _schema.end());
	return *it->second;
}


bool Schema::load (const cwt::String & uri)
{
	_dbh = DbHandler::open(uri);

	if (!_dbh.get()) {
		return false;
	}

	cwt::Vector<cwt::String> names = _dbh->tables();

	size_t ntables = names.size();

	for (size_t i = 0; i < ntables; ++i) {
		const cwt::String tname = names[i];

		// read table's meta information
		cwt::Vector<DbColumnMeta> tmeta;

		if (!_dbh->meta (tname, tmeta) ) {
			return false;
		}

		cwt::Vector<DbColumnMeta>::const_iterator itMeta = tmeta.cbegin();
		cwt::Vector<DbColumnMeta>::const_iterator itMetaEnd = tmeta.cend();

		Table & table = this->newTable(tname);

		for (; itMeta != itMetaEnd; ++itMeta) {
			Attribute & attr = table.newAttribute(itMeta->column_name, itMeta->column_type);

			attr.setPk        (itMeta->has_pk.first            ? itMeta->has_pk.second            : false);
			attr.setIndexable (itMeta->has_index.first         ? itMeta->has_index.second         : false);
			attr.setNullable  (itMeta->has_not_null.first      ? !itMeta->has_not_null.second     : false);
			attr.setUnique    (itMeta->has_unique.first        ? itMeta->has_unique.second        : false);
			attr.setUnsigned  (itMeta->has_unsigned.first      ? itMeta->has_unsigned.second      : false);
			attr.setTimestamp (itMeta->has_timestamp.first     ? itMeta->has_timestamp.second     : false);
			attr.setAutoinc   (itMeta->has_autoinc.first       ? itMeta->has_autoinc.second       : uint_t(0));
			attr.setSize      (itMeta->has_size.first          ? itMeta->has_size.second          : size_t(0));
			attr.setDefault   (itMeta->has_default_value.first ? itMeta->has_default_value.second : cwt::UniType());
		}
	}

	return true;
}

/*
Schema::~Schema()
{
	TableMap::iterator it = _schema.begin();
	TableMap::iterator itEnd = _schema.end();
	for (; it != itEnd; ++it) {
		delete it->second->ptable;
		delete it->second
		it->second = nullptr;
	}
}
*/

/*
 * @brief Enables/disables AUTOINCREMENT flag for field and sets it's value.
 *
 * @param inc Increment value. Enables flag if @c inc is greater than 0, otherwise disables it.
 * @return This field.
 *
 * @note Do not use increment value greater than 1 for compatibility, except you know
 *       that DB engine support it.
 *
 * @note MySQL: there can be only one AUTO_INCREMENT column per table, it must be indexed,
 *       and it cannot have a DEFAULT value. An AUTO_INCREMENT column works properly
 *       only if it contains only positive values. Inserting a negative number
 *       is regarded as inserting a very large positive number.
 *       This is done to avoid precision problems when numbers “wrap” over from positive
 *       to negative and also to ensure that you do not accidentally
 *       get an AUTO_INCREMENT column that contains 0.
 *
 * @note Sqlite3: It seems there is now support for value greater than 1
 *       (in the standard manner, via AUTOINCREMENT keyword).
 *
 */


} // namespace debby

CWT_NS_END



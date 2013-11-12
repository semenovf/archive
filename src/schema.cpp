/*
 * scheme.cpp
 *
 *  Created on: Sep 10, 2013
 *      Author: wladt
 */

//#include "../include/cwt/debby/dbh.hpp"
#include "../include/cwt/debby/record.hpp"
#include "../include/cwt/debby/schema.hpp"
#include <cwt/logger.hpp>
#include <cwt/safeformat.hpp>

CWT_NS_BEGIN

namespace debby
{

Schema::~Schema()
{
	RecordMap::iterator it = _schema.begin();
	RecordMap::iterator itEnd = _schema.end();
	for (; it != itEnd; ++it) {
		delete it->second;
		it->second = nullptr;
	}
}

Record & Schema::add (const cwt::String & name)
{
	CWT_ASSERT(!_schema.contains(name));
	Record * prec = new Record(name);
	_schema.insert(name, prec);
	return *prec;
}

const Record & Schema::operator [] (const cwt::String & name) const
{
	RecordMap::const_iterator it = _schema.find(name);
	return it == _schema.cend() ? _dummy : *it->second;
}

Record & Schema::operator [] (const cwt::String & name)
{
	RecordMap::const_iterator it = _schema.find(name);
	return it == _schema.cend() ? _dummy : *it->second;
}


bool Schema::deploy(const cwt::String & uri)
{
	cwt::shared_ptr<DbHandler> dbh(DbHandler::open(uri));
	if(dbh.get()) {
		return dbh->deploySchema(*dbh.get(), *this);
	}

	return false;
}


bool Schema::deploy(DbHandler & dbh)
{
	return dbh.deploySchema(dbh, *this);
}

/**
 * @brief Drop schema.
 *
 * @param name Schema (database) name.
 * @return @c true if scheme (database) successfully dropped, otherwise @c false.
 */
bool Schema::drop (const cwt::String & uri)
{
	cwt::shared_ptr<DbHandler> dbh(DbHandler::open(uri));
	if(dbh.get()) {
		return dbh->dropSchema(*dbh.get(), *this);
	}

	return false;
}

bool Schema::drop (DbHandler & dbh)
{
	return dbh.dropSchema(dbh, *this);
}


bool Schema::load (DbHandler & dbh)
{
	bool r = true;
	cwt::Vector<cwt::String> names = dbh.tables();

	size_t ntables = names.size();
	for (size_t i = 0; i < ntables; ++i) {
		Record * prec = Record::load(dbh, names[i]);
		if (!prec) {
			r = false;
			break;
		}
		_schema.insert(names[i], prec);
	}

	return r;
}

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



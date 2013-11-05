/*
 * scheme.cpp
 *
 *  Created on: Sep 10, 2013
 *      Author: wladt
 */

#include "../include/cwt/debby/schema.hpp"
#include <cwt/logger.hpp>
#include <cwt/safeformat.hpp>

CWT_NS_BEGIN

namespace debby
{

static const char * __root_object = "__debby_schema__";
const cwt::String Schema::VersionMajorKey      ("__version_major__");
const cwt::String Schema::VersionMinorKey ("__version_minor__");
static const int __version_major = 1;
static const int __version_minor = 0;

const cwt::String Schema::BoolTypeString      ("bool");
const cwt::String Schema::IntegerTypeString   ("integer");
const cwt::String Schema::FloatTypeString     ("float");
const cwt::String Schema::DoubleTypeString    ("double");
const cwt::String Schema::StringTypeString    ("string");
const cwt::String Schema::BlobTypeString      ("blob");
const cwt::String Schema::DateTypeString      ("date");
const cwt::String Schema::TimeTypeString      ("time");
const cwt::String Schema::DatetimeTypeString  ("datetime");
const cwt::String Schema::TimestampTypeString ("timestamp");
const cwt::String Schema::RefTypeString       ("ref");

Schema::Schema()
{
	JsonValue & jv = _schema.insertObject(__root_object);
	jv.insert(VersionMajorKey, __version_major);
	jv.insert(VersionMinorKey, __version_minor);
}

Table Schema::add (const cwt::String & name)
{
	JsonValue & jv = _schema[__root_object];
	CWT_ASSERT(!jv.isInvalid());
	return Table(jv.insertObject(name));
}


Field Table::addBoolean (const cwt::String & name)
{
	cwt::JsonValue & jv = _tableRef.insertObject(name);
	jv.insert("type", Schema::BoolTypeString);
	return Field(jv);
}

Field Table::addNumber (const cwt::String & name, ulong_t max)
{
	cwt::JsonValue & jv = _tableRef.insertObject(name);
	jv.insert("type", Schema::IntegerTypeString);
	jv.insert("min", 0);
	jv.insert("max", max);
	return Field(jv);
}

Field Table::addNumber (const cwt::String & name, long_t min, long_t max)
{
	cwt::JsonValue & jv = _tableRef.insertObject(name);
	jv.insert("type", Schema::IntegerTypeString);
	jv.insert("min", min);
	jv.insert("max", max);
	return Field(jv);
}

Field Table::addNumber (const cwt::String & name, double min, double max)
{
	cwt::JsonValue & jv = _tableRef.insertObject(name);

	if (min < CWT_FLOAT_MIN || max > CWT_FLOAT_MAX) {
		jv.insert("type", Schema::DoubleTypeString);
	} else {
		jv.insert("type", Schema::FloatTypeString);
	}

	return Field(jv);
}

Field Table::addString (const cwt::String & name, size_t maxLength)
{
	cwt::JsonValue & jv = _tableRef.insertObject(name);
	jv.insert("type", Schema::StringTypeString);
	jv.insert("max", maxLength);
	return Field(jv);
}

Field Table::addBlob (const cwt::String & name)
{
	cwt::JsonValue & jv = _tableRef.insertObject(name);
	jv.insert("type", Schema::BlobTypeString);
	return Field(jv);
}

Field Table::addDate (const cwt::String & name)
{
	cwt::JsonValue & jv = _tableRef.insertObject(name);
	jv.insert("type", Schema::DateTypeString);
	return Field(jv);
}

Field Table::addTime (const cwt::String & name)
{
	cwt::JsonValue & jv = _tableRef.insertObject(name);
	jv.insert("type", Schema::TimeTypeString);
	return Field(jv);
}

Field Table::addDateTime  (const cwt::String & name)
{
	cwt::JsonValue & jv = _tableRef.insertObject(name);
	jv.insert("type", Schema::DatetimeTypeString);
	return Field(jv);
}

Field Table::addTimeStamp (const cwt::String & name)
{
	cwt::JsonValue & jv = _tableRef.insertObject(name);
	jv.insert("type", Schema::TimestampTypeString);
	return Field(jv);
}

Field Table::addRef (const cwt::String & name, const cwt::String & reftable)
{
	cwt::JsonValue & jv = _tableRef.insertObject(name);
	jv.insert("type", Schema::RefTypeString);
	jv.insert("ref", reftable);
	return Field(jv);
}


Field & Field::setNullable(bool b)
{
	_columnRef.insert("nullable", b ? true : false);
	return *this;
}

/**
 *
 * @param b
 * @return
 *
 * @note Valid for MySQL: there can be only one AUTO_INCREMENT column per table, it must be indexed,
 *       and it cannot have a DEFAULT value. An AUTO_INCREMENT column works properly
 *       only if it contains only positive values. Inserting a negative number
 *       is regarded as inserting a very large positive number.
 *       This is done to avoid precision problems when numbers “wrap” over from positive
 *       to negative and also to ensure that you do not accidentally
 *       get an AUTO_INCREMENT column that contains 0.
 */
Field & Field::setAutoinc(uint_t i)
{
	_columnRef.insert("autoinc", i);
	return *this;
}

Field & Field::setPrimary(bool b)
{
	_columnRef.insert("pk", b ? true : false);
	return *this;
}

Field & Field::setUnique(bool b)
{
	_columnRef.insert("unique", b ? true : false);
	return *this;
}


} // namespace debby




#ifdef __COMMENT__
/**
 * @brief Open schema (database).
 *
 * @details Open scheme (database) specified by URI string.
 *
 * @param uri Scheme (database) uniform resource identifier.
 * @return @c false if scheme already opened or if an error occurred
 *            while opening the scheme, otherwise @c true.
 */
bool DebbyScheme::open (const String & uri)
{
	if (m_dbh) {
		Logger::error("Scheme already opened, close it before reopen.");
		return false;
	}
	m_dbh = DbHandler::open(uri);
	return m_dbh ? true : false;
}

/**
 *  @brief Close scheme.
 */
void DebbyScheme::close ()
{
	if (m_dbh) {
		delete m_dbh;    // automatically closed
		m_dbh = nullptr;
	}
}

/**
 * @fn bool Scheme::opened () const
 *
 * @brief Checks if the scheme (database) already opened.
 *
 * @return @c true if scheme (database) already opened, otherwise @c false.
 */


/**
 * @brief Drop scheme.
 *
 * @param name Scheme (database) name.
 * @return @c true if scheme (database) successfully dropped, otherwise @c false.
 */
bool DebbyScheme::drop ()
{
	bool r = true;
	if (m_dbh) {
		r = DbHandler::dropScheme(m_dbh);
		if (r) {
			delete m_dbh;
			m_dbh = nullptr;
		}
	}
	return r;
}


DebbyTable * DebbyScheme::addTable (const String & name)
{
	DebbyTables::iterator it = m_tables.insert(name, shared_ptr<DebbyTable>(new DebbyTable()));
	CWT_ASSERT(it != m_tables.end());
	DebbyTable * r = it.value().get();
	CWT_ASSERT(r);
	return r;
}

bool DebbyScheme::deploy ()
{
	return true;
}

#endif

CWT_NS_END



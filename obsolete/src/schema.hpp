/*
 * schema.hpp
 *
 *  Created on: Sep 10, 2013
 *      Author: wladt
 */

#ifndef __CWT_DEBBY_SCHEMA_HPP__
#define __CWT_DEBBY_SCHEMA_HPP__

#include <cwt/string.hpp>
#include <cwt/map.hpp>
#include <cwt/debby.hpp>
#include <cwt/debby/dbh.hpp>
#include <cwt/debby/sth.hpp>

CWT_NS_BEGIN

namespace debby
{

class Schema;
class Table;
class Record;

class DLL_API Attribute
{
public:
	typedef cwt::UniType::Type Type;

private:
	enum Bits
	{
		  Pk        = 0x0001
		, Indexable = 0x0002
		, Nullable  = 0x0004
		, Unique    = 0x0008
		, Unsigned  = 0x0010
		, Timestamp = 0x0020
	};

	struct Meta
	{
		uint32_t     _bits;
		uint_t       _autoinc;
		size_t       _size;
		cwt::UniType _default;

		Meta() : _bits(0), _autoinc(0), _size(0) {}
	};

	Attribute (Type type);
	Attribute (const cwt::UniType & value) : _value(value) {}

public:
	Attribute () : _value() {}
	Attribute & operator = (const cwt::UniType & value) { _value = value; return *this; }

	bool isInvalid () const { return _value.isNull(); }
	Type type () const { return _value.type(); }

	Attribute & setPk        (bool b = true);
	Attribute & setIndexable (bool b = true);
	Attribute & setNullable  (bool b = true);
	Attribute & setUnique    (bool b = true);
	Attribute & setUnsigned  (bool b = true);
	Attribute & setTimestamp (bool b = true);
	Attribute & setAutoinc   (uint_t n = 1);
	Attribute & setSize      (size_t size = 0);
	Attribute & setDefault   (const cwt::UniType & def = cwt::UniType());

	bool isPk        () const { return _meta._bits & Pk; }
	bool isIndexable () const { return _meta._bits & Indexable; }
	bool isNullable  () const { return _meta._bits & Nullable; }
	bool isUnique    () const { return _meta._bits & Unique; }
	bool isUnsigned  () const { return _meta._bits & Unsigned; }
	bool isTimestamp () const { return _meta._bits & Timestamp; }
	bool isAutoinc   () const { return _meta._autoinc != 0; }
	uint_t autoinc   () const { return _meta._autoinc; }
	bool hasDefault () const  { return !_meta._default.isNull(); }
	cwt::UniType defaultValue () const { return _meta._default; }

	cwt::String stringify () const;

private:
	cwt::UniType _value;
	Meta         _meta;

	friend class Table;
};

class DLL_API Table
{
	enum PreparedStmt
	{
		  SthForCreate
		, SthForUpdate
		, SthForDelete
	};

	typedef cwt::shared_ptr<Attribute> AttributePtr;
	typedef cwt::Map<cwt::String, AttributePtr> AttributeMap;

	cwt::String buildSqlForCreateRecord ();
	cwt::String buildSqlForUpdateRecord ();
	bool createRecord (const Record & record);
	bool updateRecord (const Record & record);
	bool deleteRecord (const Record & record);

public:
	const String & name () const { return _name; }

	Attribute & newAttribute (const cwt::String & attrname, Attribute::Type attrtype);
	const Attribute & operator [] (const cwt::String & name) const;
	Attribute & operator [] (const cwt::String & name);

	bool contains (const cwt::String & name) { return _columns.contains(name); }
	size_t count () const { return _columns.size(); }
	cwt::Vector<cwt::String> names() const { return _columns.keys(); }

	/*
	static RecordPtr findBy (DbHandler & dbh, const cwt::String & name
			, const cwt::String & a, const cwt::UniType & v);
	static RecordPtr findBy (DbHandler & dbh, const cwt::String & name
			, const cwt::String & a0, const cwt::UniType & v0
			, const cwt::String & a1, const cwt::UniType & v1);
	static RecordPtr findBy (DbHandler & dbh, const cwt::String & name
			, const cwt::String & a0, const cwt::UniType & v0
			, const cwt::String & a1, const cwt::UniType & v1
			, const cwt::String & a2, const cwt::UniType & v3);
*/
/*
	static RecordPtr findBy (DbHandler & dbh, const cwt::String & name
			, int argc, const std::pair<cwt::String, cwt::UniType> args[]);
*/

private:
	//std::pair<cwt::String, Attribute *> _cachedPk;
	cwt::String   _name;
	AttributeMap  _columns;
	StatementPtr  _prepared[3];
	Schema      * _pschema;

	friend class Schema;
	friend class Record;
};

class DLL_API Schema
{
	typedef cwt::shared_ptr<Table> TablePtr;
	typedef cwt::Map<cwt::String, TablePtr> TableMap;

public:
	Schema () {}

	bool load (const cwt::String & uri);
	bool save ();

	Table & newTable (const cwt::String & tname);
	const Table & operator [] (const cwt::String & name) const;
	Table & operator [] (const cwt::String & name);

	bool contains (const cwt::String & tname) { return _schema.contains(tname); }
	size_t count () const { return _schema.size(); }
	cwt::Vector<cwt::String> names() const { return _schema.keys(); }

//	bool create (const cwt::String & uri);

	bool begin () { return _dbh->begin(); }
	bool commit () { return _dbh->commit(); }
	bool rollback () { return _dbh->rollback(); }

private:
	DbHandlerPtr _dbh;
	TableMap     _schema;
};

} // namespace debby

CWT_NS_END

#endif /* __CWT_DEBBY_SCHEMA_HPP__ */

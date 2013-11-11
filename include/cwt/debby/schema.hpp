/*
 * scheme.hpp
 *
 *  Created on: Sep 10, 2013
 *      Author: wladt
 */

#ifndef __CWT_DEBBY_SCHEME_HPP__
#define __CWT_DEBBY_SCHEME_HPP__

#include <cwt/string.hpp>
//#include <cwt/json.hpp>
#include <cwt/map.hpp>
//#include <cwt/shared_ptr.hpp>
#include <cwt/debby.hpp>
#include <cwt/debby/dbh.hpp>
//#include <cwt/debby/record.hpp>
//#include <cwt/debby/sth.hpp>

CWT_NS_BEGIN

namespace debby
{

class Record;

class DLL_API Schema
{
public:
	Schema() {}

	Schema & operator << (const Record & record) { return add(record); }
	Schema & add (const Record & record) { _schema.insert(record.name(), & record); return *this; }

//	Table add (const cwt::String & name);

	bool deploy(const cwt::String & uri);
	bool deploy(DbHandler & dbh);
	bool drop(const cwt::String & uri);
	bool drop(DbHandler & dbh);

	uint_t version (ushort_t * major = nullptr, ushort_t * minor = nullptr) const;

	size_t count () const { return _schema.size(); }
	cwt::Vector<cwt::String> names() const { return _schema.keys(); }
	const Record & operator [] (const cwt::String & name) const
	{
		cwt::Map<cwt::String, const Record *>::const_iterator it = _schema.find(name);
		return it == _schema.cend() ? _dummy : *it->second;
	}

private:
	cwt::Map<cwt::String, const Record *> _schema;
	Record _dummy;
};


#ifdef __COMMENT__

class Schema;
class Table;

class DLL_API Field
{
public:
	static const bool Unsigned = true;
	static const bool Signed = false;

protected:
	Field(const cwt::String & name, const cwt::JsonValue & column) : _name(name), _column(column) {}
	Field() : _name(), _column(cwt::JsonValue::sharedNull) {}

public:
	Field & setNullable (bool b = true);
	Field & setUnique   (bool b = true);
	Field & setAutoinc  (uint_t inc = 1);

	bool isNullable () const;
	bool isUnique   () const;
	bool isUnsigned () const;
	bool isRef      () const;
	bool isAutoinc  () const;
	bool isPk       () const;

	bool isInvalid() const;
	cwt::UniType::TypeEnum type () const;
	Field deref() const;

private:
	const cwt::String _name;
	cwt::JsonValue    _column;

	friend class Table;
};


class DLL_API Table
{
protected:
	Table(const JsonValue & table) : _table(table) {}
	Table() : _table(cwt::JsonValue::sharedNull) {}

public:
	Field addBoolean   (const cwt::String & name);
	Field addInteger   (const cwt::String & name, size_t size = 0, bool isUnsigned = false);
	Field addDecimal   (const cwt::String & name, size_t size = 0, size_t decimals = 0, bool isUnsigned = false);
	Field addFloat     (const cwt::String & name, bool isUnsigned = false);
	Field addDouble    (const cwt::String & name, bool isUnsigned = false);
	Field addString    (const cwt::String & name, size_t size = 0);
	Field addBlob      (const cwt::String & name);
	Field addDate      (const cwt::String & name, bool isTimeStamp = false);
	Field addTime      (const cwt::String & name, bool isTimeStamp = false);
	Field addDateTime  (const cwt::String & name, bool isTimeStamp = false);
	Field addRef       (const cwt::String & name, const cwt::String & reftable);

	void addPrimaryKey   (const cwt::String & fieldname);
	void setPrimaryKey   (const cwt::String & fieldname);
	void clearPrimaryKey ();

	size_t count () const;
	cwt::Vector<cwt::String> fieldNames() const;
	Field field(const cwt::String & name) const;
	cwt::Vector<cwt::String> primaryKeyNames() const;

private:
	JsonValue _table;

	friend class Schema;
};


typedef cwt::UniType                 Value;
typedef cwt::Map<cwt::String, Value> Row;
typedef cwt::Vector<Row>             RowSet;

class DLL_API Select
{
public:
	Select(const cwt::String & result = cwt::String());

	Select & distinct (bool b = true);
	Select & limit (uint_t from = 1, uint_t to = 1);

	cwt::String result () const;
	bool isDistinct () const;
	void limit (uint_t * from, uint_t * to) const;

	Row first (DbHandler & dbh);
	Row last (DbHandler & dbh);

private:
	Json _query;
};

class DLL_API Insert
{
public:
	Insert(const cwt::String & target) : _isReady(false), _target(target) {}

	Value & operator [] (const cwt::String & column);
	const Value operator [] (const cwt::String & column) const { return _record[column]; }
//	Insert & bind

//	Insert & where(const String & expr);
	bool save (DbHandler & dbh);

private:
	cwt::String buildSql() const;

private:
	bool        _isReady;
	cwt::String _target;
	Row         _record;
	Value       _dummy;
	cwt::shared_ptr<DbStatement> _sth;
};


class DLL_API Update
{
public:
	Update(const cwt::String & target) : _isReady(false), _target(target) {}

	Value & operator [] (const cwt::String & column);
	const Value operator [] (const cwt::String & column) const { return _record[column]; }

	Update & where(const String & expr);
	bool update (DbHandler & dbh);

private:
	cwt::String buildSql() const;

private:
	bool        _isReady;
	cwt::String _target;
	cwt::String _where;
	Row         _record;
	Value       _dummy;
	cwt::shared_ptr<DbStatement> _sth;
};

class DLL_API Schema
{
public:
	Schema();

	Table add (const cwt::String & name);

	bool deploy(const cwt::String & uri);
	bool deploy(DbHandler & dbh);
	bool drop(const cwt::String & uri);
	bool drop(DbHandler & dbh);

	const JsonValue & json () const { return _schema; }
	uint_t version (ushort_t * major = nullptr, ushort_t * minor = nullptr) const;

	size_t count () const;
	cwt::Vector<cwt::String> tableNames() const;
	Table table(const cwt::String & name) const;

private:
	Json _schema;
};

#endif

} // namespace debby

CWT_NS_END

#endif /* __CWT_DEBBY_SCHEME_HPP__ */

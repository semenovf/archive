/*
 * record.hpp
 *
 *  Created on: Nov 11, 2013
 *      Author: wladt
 */

#ifndef __CWT_DEBBY_RECORD_HPP__
#define __CWT_DEBBY_RECORD_HPP__

#include <cwt/string.hpp>
#include <cwt/map.hpp>
#include <cwt/unitype.hpp>
#include <cwt/debby.hpp>
#include <cwt/debby/dbh.hpp>
#include <cwt/debby/sth.hpp>

CWT_NS_BEGIN

namespace debby
{

class Record;

class DLL_API Attribute
{
	typedef cwt::Map<cwt::String, cwt::UniType> MetaType;

public:
	static const bool Unsigned = true;
	static const bool Signed = false;

private:
	Attribute () {}
	Attribute (const cwt::String & name, const cwt::UniType & value);
	bool isBooleanSet (const cwt::String & optname) const;
	bool hasOpt       (const cwt::String & optname) const;

public:
	const String & name () const { return _name; }
	bool isInvalid() const { return _name.isEmpty(); }

	Attribute & setNullable (bool b = true);
	Attribute & setUnique   (bool b = true);
	Attribute & setDefault  (const cwt::UniType & defaultValue);

	bool isPk () const;
	bool isAutoinc  () const;
	uint_t autoinc  () const;
	bool isNullable () const;
	bool isUnique   () const;
	bool isUnsigned () const;
	bool hasDefault () const;
	cwt::UniType value () const { return _value; }
	cwt::UniType defaultValue () const;
	cwt::UniType::TypeEnum type () const;

	Attribute & operator = (const cwt::UniType & value) { _value = value; return *this; }

private:
	cwt::String  _name;
	cwt::UniType _value;
	MetaType     _meta;

	friend class Record;
};

class DLL_API Record
{
	typedef cwt::Map<cwt::String, Attribute *> AttributeMap;

public:
	static const cwt::String PkName;

protected:
	cwt::String buildSqlForCreate () const;

public:
	Record();
	Record (const cwt::String & name, cwt::UniType::TypeEnum pkType = cwt::UniType::IntegerValue);
	~Record ();
	const String & name () const { return _name; }
	Attribute & addBoolean   (const cwt::String & name);
	Attribute & addInteger   (const cwt::String & name, size_t size = 0, bool isUnsigned = false);
	Attribute & addDecimal   (const cwt::String & name, size_t size = 0, size_t decimals = 0, bool isUnsigned = false);
	Attribute & addFloat     (const cwt::String & name, bool isUnsigned = false);
	Attribute & addDouble    (const cwt::String & name, bool isUnsigned = false);
	Attribute & addString    (const cwt::String & name, size_t size = 0);
	Attribute & addBlob      (const cwt::String & name);
	Attribute & addDate      (const cwt::String & name, bool isTimeStamp = false);
	Attribute & addTime      (const cwt::String & name, bool isTimeStamp = false);
	Attribute & addDateTime  (const cwt::String & name, bool isTimeStamp = false);
	Attribute & addRef       (const cwt::String & name, const Record & refrecord);

	bool isInvalid() const { return _name.isEmpty(); }
	const Attribute & pk () const { return _pk; }
	void setAutoinc (uint_t n = 1);
	uint_t autoinc () const;

	size_t count () const { return _attrs.size(); }
	cwt::Vector<cwt::String> names() const { return _attrs.keys(); }
	const Attribute & operator [] (const cwt::String & name) const;
	Attribute & operator [] (const cwt::String & name);

	bool create (DbHandler & dbh);
/*
	bool load();

	bool update();
	bool del();
*/

private:
	cwt::String  _name;
	Attribute    _pk;
	Attribute    _dummy;
	AttributeMap _attrs;
	DbStatement  * _sthPool[4]; // 0 - for load, 1 - for create, 2 - for update, 3 - for delete
};


typedef Record Table;
typedef Attribute Field;


inline Record::Record()
    : _name()
	, _pk()
{
	_sthPool[0] = _sthPool[1] = _sthPool[2] = _sthPool[3] = nullptr;
}


/*
typedef cwt::UniType                 Value;
typedef cwt::Map<cwt::String, Value> Row;
typedef cwt::Vector<Row>             RowSet;
*/

/*class DLL_API Select
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
};*/

} // namespace debby

CWT_NS_END

#endif /* __CWT_DEBBY_SCHEME_HPP__ */

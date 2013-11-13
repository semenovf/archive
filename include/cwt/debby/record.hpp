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

class Attribute;
class Record;
class Schema;

typedef Record Table;
typedef Attribute Field;

class DLL_API Attribute
{
	typedef cwt::Map<cwt::String, cwt::UniType> MetaType;

	Attribute () {}
	Attribute (const Attribute & other) : _value(other._value), _meta(other._meta)  {}
	Attribute & operator = (const Attribute & other) { _value = other._value; _meta = other._meta; return *this; }

	bool isBooleanSet (const cwt::String & optname) const;
	bool hasOpt       (const cwt::String & optname) const;

public:
	static const bool Unsigned = true;
	static const bool Signed = false;

	Attribute & setPk       (bool b = true);
	Attribute & setAutoinc  (uint_t n = 1);
	Attribute & setNullable (bool b = true);
	Attribute & setUnique   (bool b = true);
	Attribute & setDefault  (const cwt::UniType & defaultValue);
	Attribute & setValue    (const cwt::UniType & value) { _value = value; return *this; }

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

	Attribute & operator = (const cwt::UniType & value) { return setValue(value); }

	static cwt::String stringifyValue(const Attribute & attr);

private:
	cwt::UniType _value;
	MetaType     _meta;

	friend class Record;
};

class DLL_API Record
{
	typedef cwt::Map<cwt::String, Attribute *> AttributeMap;
	enum {
		  SthForCreate
		, SthForUpdate
		, SthForDestroy
	};

	Attribute & add (const cwt::String & name);
	cwt::String buildSqlForCreate () const;
	cwt::String buildSqlForDestroy (); // not const
	cwt::String buildSqlForLoad () const;
	static Record * load (DbHandler & dbh, const cwt::String & name);

	Record () : _cachedPk(cwt::String(), nullptr), _name() {}
	Record (const cwt::String & name) : _cachedPk(cwt::String(), nullptr), _name(name) {}

public:
	~Record ();
	const String & name      () const { return _name; }
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
	Attribute & addFromAttr  (const cwt::String & name, const Attribute & attr);

	size_t count () const { return _attrs.size(); }
	cwt::Vector<cwt::String> names() const { return _attrs.keys(); }

	const Attribute & operator [] (const cwt::String & name) const;
	Attribute &       operator [] (const cwt::String & name);
	bool containes (const cwt::String & name) { return _attrs.contains(name); }

	bool create (DbHandler & dbh);
	bool destroy (DbHandler & dbh);
	/*
		bool update();
	*/

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
	std::pair<cwt::String, Attribute *> _cachedPk;
	cwt::String   _name;
	Attribute     _dummy;
	AttributeMap  _attrs;
	StatementPtr  _sthPool[3]; // 0 - for create, 1 - for update, 2 - for delete

	friend class Schema;
};

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
*/
} // namespace debby

CWT_NS_END

#endif /* __CWT_DEBBY_SCHEME_HPP__ */

/**
 * @file schema.hpp

 * @author wladt
 * @date Feb 10, 2014
 *
 * @note Based on [hiberlite](https://code.google.com/p/hiberlite/) library
 */

#ifndef __PFS_DEBBY_SCHEMA_HPP__
#define __PFS_DEBBY_SCHEMA_HPP__

#include <pfs/debby/database.hpp>
#include <pfs/debby/statement.hpp>
#include <pfs/date.hpp>
#include <pfs/time.hpp>
#include <pfs/datetime.hpp>

namespace pfs { namespace debby {

class schema;
class table_basic;


template <typename T>
string sql_type_str ();

template<> string sql_type_str<bool> ()               { return _l1("BOOLEAN"); }
template<> string sql_type_str<char> ()               { return _l1("CHAR(1)"); }
template<> string sql_type_str<unsigned char> ()      { return _l1("CHAR(1)"); }
template<> string sql_type_str<short> ()              { return _l1("INTEGER"); }
template<> string sql_type_str<unsigned short> ()     { return _l1("INTEGER UNSIGNED"); }
template<> string sql_type_str<int> ()                { return _l1("INTEGER"); }
template<> string sql_type_str<unsigned int> ()       { return _l1("INTEGER UNSIGNED"); }
template<> string sql_type_str<long> ()               { return _l1("INTEGER"); }
template<> string sql_type_str<unsigned long> ()      { return _l1("INTEGER UNSIGNED"); }
template<> string sql_type_str<long long> ()          { return _l1("INTEGER"); }
template<> string sql_type_str<unsigned long long> () { return _l1("INTEGER UNSIGNED"); }
template<> string sql_type_str<float> ()              { return _l1("FLOAT"); }
template<> string sql_type_str<double> ()             { return _l1("DOUBLE"); }
template<> string sql_type_str<string> ()             { return _l1("TEXT"); }
template<> string sql_type_str<byte_string> ()        { return _l1("BLOB"); }
template<> string sql_type_str<date> ()               { return _l1("DATE"); }
template<> string sql_type_str<time> ()               { return _l1("TIME"); }
template<> string sql_type_str<datetime> ()           { return _l1("DATETIME"); }

class field_basic
{
	table_basic * _holder;
	string   _name;
	bool          _isPk; // primary key
	string   _sqlType;

protected:
	field_basic (const string & name, const string & sqlType, table_basic & holder)
		: _holder(& holder)
		, _name(name)
		, _isPk(false)
		, _sqlType(sqlType)
	{}

public:
	bool isPk () const { return _isPk; }
	const string & name () const { return _name; }

	void setPk (bool b = true) { _isPk = b; }
};

template <typename T>
class field : public field_basic
{
	T * _ptr;
public:
	field (T & ptr, const string & name, table_basic & holder)
		: field_basic(name, sql_type_str<T>(), holder)
		, _ptr(& ptr)
	{}
};

class table_basic
{
	typedef shared_ptr<field_basic> value_type;
	typedef map<string, value_type> fields_map_type;

	string     _name;
	schema *        _schema;
	fields_map_type _fields;

public:
	table_basic (const string & name, schema & holder) : _name(name), _schema(& holder) {}

protected:
	template <typename T>
	field_basic & addScalarField (const string & name, T & ref)
	{
		field<T> * f = new field<T>(ref, name, *this);
		_fields.insert(name, value_type(f));
		return *f;
	}

	bool deploy (database & db);

public:
	template <typename T>
	field_basic & addField (const string & name, T & ref);

	template <typename T>
	void addField (const string & /*name*/, vector<T> &) {  }

	template <typename Key, typename T>
	void addField (const string & /*name*/, map<Key, T> &) { }
};

template <typename Class>
class table : public table_basic
{
	Class _buffer;
public:
	table (const string & name, schema & holder) : table_basic(name, holder) {}
	void persist () {}

	Class & operator () () { return _buffer; }
	const Class & operator () () const { return _buffer; }
};

class schema
{
	typedef shared_ptr<table_basic> value_type;

	map<string, value_type> _tables;

public:
	schema () {}
};

template <>
inline field_basic & table_basic::addField<bool> (const string & name, bool & ar)
{
	return addScalarField(name, ar);
}

template <>
inline field_basic & table_basic::addField<char> (const string & name, char & ar)
{
	return addScalarField(name, ar);
}

template <>
inline field_basic & table_basic::addField<unsigned char> (const string & name, unsigned char & ar)
{
	return addScalarField(name, ar);
}

template <>
inline field_basic & table_basic::addField<short> (const string & name, short & ar)
{
	return addScalarField(name, ar);
}

template <>
inline field_basic & table_basic::addField<int> (const string & name, int & ar)
{
	return addScalarField(name, ar);
}

template <>
inline field_basic & table_basic::addField<string> (const string & name, string & ar)
{
	return addScalarField(name, ar);
}

template <>
inline field_basic & table_basic::addField<byte_string> (const string & name, byte_string & ar)
{
	return addScalarField(name, ar);
}

template <>
inline field_basic & table_basic::addField<date> (const string & name, date & ar)
{
	return addScalarField(name, ar);
}

template <>
inline field_basic & table_basic::addField<time> (const string & name, time & ar)
{
	return addScalarField(name, ar);
}

template <>
inline field_basic & table_basic::addField<datetime> (const string & name, datetime & ar)
{
	return addScalarField(name, ar);
}

#ifdef __COMMENT__
class column
{
	string _name;
	string _storageType;

public:
	column (const string & name = string()
			, const string & storageType = string())
		: _name(name)
		, _storageType(storageType) {}

	string name        () const { return _name; }
	string storageType () const { return _storageType; }
};

class table
{
	string _name;
	map<string, column> _columns;

public:
	table ();

	string name () const { return _name; }
	void add (const column & col);
	bool contains (const string & colname) const;
};


class schema : public map<string, table>, public errorable
{
	//hiberlite::ModelExtractor * mx;
public:
	static const string PrimaryKeyColumn;
	static const string PrimaryKeyStorageType;

public:
	virtual ~schema () {}

	void add (table t);
	void drop ();
	void create ();
};

#endif

}} // pfs::debby

#endif /* __PFS_DEBBY_SCHEMA_HPP__ */

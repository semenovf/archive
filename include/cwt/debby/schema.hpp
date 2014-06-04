/**
 * @file schema.hpp

 * @author wladt
 * @date Feb 10, 2014
 *
 * @note Based on [hiberlite](https://code.google.com/p/hiberlite/) library
 */

#ifndef __CWT_DEBBY_SCHEMA_HPP__
#define __CWT_DEBBY_SCHEMA_HPP__

#include <cwt/debby/database.hpp>
#include <cwt/debby/statement.hpp>
#include <pfs/date.hpp>
#include <pfs/time.hpp>
#include <pfs/datetime.hpp>


namespace cwt { namespace debby {

class schema;
class table_basic;


template <typename T>
pfs::string sql_type_str ();

template<> pfs::string sql_type_str<bool> ()               { return _l1("BOOLEAN"); }
template<> pfs::string sql_type_str<char> ()               { return _l1("CHAR(1)"); }
template<> pfs::string sql_type_str<unsigned char> ()      { return _l1("CHAR(1)"); }
template<> pfs::string sql_type_str<short> ()              { return _l1("INTEGER"); }
template<> pfs::string sql_type_str<unsigned short> ()     { return _l1("INTEGER UNSIGNED"); }
template<> pfs::string sql_type_str<int> ()                { return _l1("INTEGER"); }
template<> pfs::string sql_type_str<unsigned int> ()       { return _l1("INTEGER UNSIGNED"); }
template<> pfs::string sql_type_str<long> ()               { return _l1("INTEGER"); }
template<> pfs::string sql_type_str<unsigned long> ()      { return _l1("INTEGER UNSIGNED"); }
template<> pfs::string sql_type_str<long long> ()          { return _l1("INTEGER"); }
template<> pfs::string sql_type_str<unsigned long long> () { return _l1("INTEGER UNSIGNED"); }
template<> pfs::string sql_type_str<float> ()              { return _l1("FLOAT"); }
template<> pfs::string sql_type_str<double> ()             { return _l1("DOUBLE"); }
template<> pfs::string sql_type_str<pfs::string> ()        { return _l1("TEXT"); }
template<> pfs::string sql_type_str<pfs::bytearray> ()     { return _l1("BLOB"); }
template<> pfs::string sql_type_str<pfs::date> ()          { return _l1("DATE"); }
template<> pfs::string sql_type_str<pfs::time> ()          { return _l1("TIME"); }
template<> pfs::string sql_type_str<pfs::datetime> ()      { return _l1("DATETIME"); }

class field_basic
{
	table_basic * _holder;
	pfs::string   _name;
	bool          _isPk; // primary key
	pfs::string   _sqlType;

protected:
	field_basic (const pfs::string & name, const pfs::string & sqlType, table_basic & holder)
		: _holder(& holder)
		, _name(name)
		, _isPk(false)
		, _sqlType(sqlType)
	{}

public:
	bool isPk () const { return _isPk; }
	const pfs::string & name () const { return _name; }

	void setPk (bool b = true) { _isPk = b; }
};

template <typename T>
class field : public field_basic
{
	T * _ptr;
public:
	field (T & ptr, const pfs::string & name, table_basic & holder)
		: field_basic(name, sql_type_str<T>(), holder)
		, _ptr(& ptr)
	{}
};

class table_basic
{
	typedef pfs::shared_ptr<field_basic> value_type;
	typedef pfs::map<pfs::string, value_type> fields_map_type;

	pfs::string     _name;
	schema *        _schema;
	fields_map_type _fields;

public:
	table_basic (const pfs::string & name, schema & holder) : _name(name), _schema(& holder) {}

protected:
	template <typename T>
	field_basic & addScalarField (const pfs::string & name, T & ref)
	{
		field<T> * f = new field<T>(ref, name, *this);
		_fields.insert(name, value_type(f));
		return *f;
	}

	bool deploy (cwt::debby::database & db);

public:
	template <typename T>
	field_basic & addField (const pfs::string & name, T & ref);

	template <typename T>
	void addField (const pfs::string & /*name*/, pfs::vector<T> &) {  }

	template <typename Key, typename T>
	void addField (const pfs::string & /*name*/, pfs::map<Key, T> &) { }
};

template <typename Class>
class table : public table_basic
{
	Class _buffer;
public:
	table (const pfs::string & name, schema & holder) : table_basic(name, holder) {}
	void persist () {}

	Class & operator () () { return _buffer; }
	const Class & operator () () const { return _buffer; }
};

class schema
{
	typedef pfs::shared_ptr<table_basic> value_type;

	pfs::map<pfs::string, value_type> _tables;

public:
	schema () {}
};

template <>
inline field_basic & table_basic::addField<bool> (const pfs::string & name, bool & ar)
{
	return addScalarField(name, ar);
}

template <>
inline field_basic & table_basic::addField<char> (const pfs::string & name, char & ar)
{
	return addScalarField(name, ar);
}

template <>
inline field_basic & table_basic::addField<unsigned char> (const pfs::string & name, unsigned char & ar)
{
	return addScalarField(name, ar);
}

template <>
inline field_basic & table_basic::addField<short> (const pfs::string & name, short & ar)
{
	return addScalarField(name, ar);
}

template <>
inline field_basic & table_basic::addField<int> (const pfs::string & name, int & ar)
{
	return addScalarField(name, ar);
}

template <>
inline field_basic & table_basic::addField<pfs::string> (const pfs::string & name, pfs::string & ar)
{
	return addScalarField(name, ar);
}

template <>
inline field_basic & table_basic::addField<pfs::bytearray> (const pfs::string & name, pfs::bytearray & ar)
{
	return addScalarField(name, ar);
}

template <>
inline field_basic & table_basic::addField<pfs::date> (const pfs::string & name, pfs::date & ar)
{
	return addScalarField(name, ar);
}

template <>
inline field_basic & table_basic::addField<pfs::time> (const pfs::string & name, pfs::time & ar)
{
	return addScalarField(name, ar);
}

template <>
inline field_basic & table_basic::addField<pfs::datetime> (const pfs::string & name, pfs::datetime & ar)
{
	return addScalarField(name, ar);
}

#ifdef __COMMENT__
class column
{
	pfs::string _name;
	pfs::string _storageType;

public:
	column (const pfs::string & name = pfs::string()
			, const pfs::string & storageType = pfs::string())
		: _name(name)
		, _storageType(storageType) {}

	pfs::string name        () const { return _name; }
	pfs::string storageType () const { return _storageType; }
};

class table
{
	pfs::string _name;
	pfs::map<pfs::string, column> _columns;

public:
	table ();

	pfs::string name () const { return _name; }
	void add (const column & col);
	bool contains (const pfs::string & colname) const;
};


class schema : public pfs::map<pfs::string, table>, public errorable
{
	//hiberlite::ModelExtractor * mx;
public:
	static const pfs::string PrimaryKeyColumn;
	static const pfs::string PrimaryKeyStorageType;

public:
	virtual ~schema () {}

	void add (table t);
	void drop ();
	void create ();
};

#endif

}} // cwt::debby

#endif /* __CWT_DEBBY_SCHEMA_HPP__ */

/*
 * scheme.hpp
 *
 *  Created on: Sep 10, 2013
 *      Author: wladt
 */

#ifndef __CWT_DEBBY_SCHEME_HPP__
#define __CWT_DEBBY_SCHEME_HPP__

#include <cwt/string.hpp>
#include <cwt/json.hpp>
#include <cwt/debby.hpp>

CWT_NS_BEGIN

namespace debby
{

class Schema;
class Table;

class DLL_API Field
{
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


class DLL_API Schema
{
public:
	Schema();

	Table add (const cwt::String & name);

	bool deploy(const cwt::String & uri);
	bool drop(const cwt::String & uri);
	const JsonValue & json () const { return _schema; }
	uint_t version (ushort_t * major = nullptr, ushort_t * minor = nullptr) const;

	size_t count () const;
	cwt::Vector<cwt::String> tableNames() const;
	Table table(const cwt::String & name) const;

private:
	Json _schema;
};

} // namespace debby

CWT_NS_END

#endif /* __CWT_DEBBY_SCHEME_HPP__ */

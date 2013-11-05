/*
 * scheme.hpp
 *
 *  Created on: Sep 10, 2013
 *      Author: wladt
 */

#ifndef __CWT_DEBBY_SCHEME_HPP__
#define __CWT_DEBBY_SCHEME_HPP__

#include <cwt/string.hpp>
#include <cwt/debby.hpp>
#include <cwt/debby/dbh.hpp>
#include <cwt/json.hpp>

CWT_NS_BEGIN

namespace debby {

class Schema;
class Table;

class DLL_API Field
{
protected:
	Field(JsonValue & columnRef) : _columnRef(columnRef) {}

public:
	Field & setNullable(bool b = true);
	Field & setAutoinc(uint_t i = 1);
	Field & setPrimary(bool b = true);
	Field & setUnique(bool b = true);

private:
	JsonValue & _columnRef;

	friend class Table;
};


class DLL_API Table
{
protected:
	Table(JsonValue & tableRef) : _tableRef(tableRef) {}

public:
	Field addBoolean   (const cwt::String & name);
	Field addNumber    (const cwt::String & name, ulong_t max);
	Field addNumber    (const cwt::String & name, long_t min, long_t max);
	Field addNumber    (const cwt::String & name, double min, double max);
	Field addString    (const cwt::String & name, size_t maxLength = CWT_SIZE_MAX);
	Field addBlob      (const cwt::String & name);
	Field addDate      (const cwt::String & name);
	Field addTime      (const cwt::String & name);
	Field addDateTime  (const cwt::String & name);
	Field addTimeStamp (const cwt::String & name);
	Field addRef       (const cwt::String & name, const cwt::String & reftable);

private:
	JsonValue & _tableRef;

	friend class Schema;
};


class DLL_API Schema
{
	CWT_DENY_COPY(Schema);

public:
	static const cwt::String BoolTypeString;
	static const cwt::String IntegerTypeString;
	static const cwt::String FloatTypeString;
	static const cwt::String DoubleTypeString;
	static const cwt::String StringTypeString;
	static const cwt::String BlobTypeString;
	static const cwt::String DateTypeString;
	static const cwt::String TimeTypeString;
	static const cwt::String DatetimeTypeString;
	static const cwt::String TimestampTypeString;
	static const cwt::String RefTypeString;

	static const cwt::String VersionMajorKey;
	static const cwt::String VersionMinorKey;

public:
	Schema();
	bool create ();
	bool destroy ();

	Table add (const cwt::String & name);

	bool deploy(const cwt::String & uri);
	const Json & json () const { return _schema; }

private:
	Json _schema;
};

} // namespace debby

CWT_NS_END

#endif /* __CWT_DEBBY_SCHEME_HPP__ */

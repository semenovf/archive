/*
 * scheme.hpp
 *
 *  Created on: Sep 10, 2013
 *      Author: wladt
 */

#ifndef __CWT_DEBBY_SCHEME_HPP__
#define __CWT_DEBBY_SCHEME_HPP__

#include <cwt/string.hpp>
#include <cwt/shared_ptr.hpp>
#include <cwt/unitype.hpp>
#include <cwt/orderedhash.hpp>
#include <cwt/debby.hpp>
#include <cwt/debby/dbh.hpp>

CWT_NS_BEGIN

class DebbyTable;

class DLL_API DebbyField
{
public:
	DebbyField()
		: m_type(DebbyNull)
		, m_pk(false)
		, m_autoinc(0)
		, m_nullable(false)
		, m_reftable() {}

	DebbyField(DebbyTypeEnum type)
		: m_type(type)
		, m_pk(false)
		, m_autoinc(0)
		, m_nullable(false)
		, m_reftable() {}

	void setPk(bool b) { m_pk = b; }
	void setAutoinc(int n) { m_autoinc = n; }
	void setNullable(bool b) { m_nullable = b; }
	void setRefTable(const String & reftable) { m_reftable = reftable; }

private:
	DebbyTypeEnum m_type;
	bool m_pk;
	int m_autoinc;
	bool m_nullable;
	String m_reftable;
};

class DLL_API DebbyTable
{
	typedef OrderedHash<String, shared_ptr<DebbyField> > DebbyFields;
public:
	DebbyTable () : m_fields() {}
	DebbyField * addBooleanField   (const String & name);
	DebbyField * addNumberField    (const String & name, ulong_t max);
	DebbyField * addNumberField    (const String & name, long_t min, long_t max);
	DebbyField * addNumberField    (const String & name, double min, double max);
	DebbyField * addStringField    (const String & name, size_t maxLength = CWT_SIZE_MAX);
	DebbyField * addBlobField      (const String & name);
	DebbyField * addDateField      (const String & name);
	DebbyField * addTimeField      (const String & name);
	DebbyField * addDateTimeField  (const String & name);
	DebbyField * addTimeStampField (const String & name);
	DebbyField * addRefField       (const String & name, const String & reftable);

private:
	DebbyField * addField (const String & name, DebbyField * field);

private:
	DebbyFields m_fields;
};


class DLL_API DebbyScheme
{
	typedef OrderedHash<String, shared_ptr<DebbyTable> > DebbyTables;
	CWT_DENY_COPY(DebbyScheme);
public:
	DebbyScheme () : m_dbh(nullptr) {}
	DebbyScheme (const String & uri) : m_dbh(nullptr) { open(uri); }

	void close ();
	bool open (const String & uri);
	bool opened () const { return m_dbh == nullptr ? false : m_dbh->opened(); }

	DebbyTable * addTable (const String & name);

	// bool populate(); // populate from scheme
	bool deploy ();
	bool drop ();

private:
	DbHandler * m_dbh;
	DebbyTables m_tables;
};

CWT_NS_END

#endif /* __CWT_DEBBY_SCHEME_HPP__ */

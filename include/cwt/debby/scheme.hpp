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

namespace debby {

class DLL_API Field
{
public:
	Field() : m_type(Debby::TypeNull) {}
	void setBoolean ();
	void setNumber (ulong_t max);
	void setNumber (long_t min, long_t max);
	void setNumber (double min, double max);

private:
	Debby::TypeEnum m_type;
};

class DLL_API Table
{
	typedef OrderedHash<String, shared_ptr<Field> > Fields;
public:
	Table () : m_fields() {}
	Field * addField (const String & name);
private:
	Fields m_fields;
};


class DLL_API Scheme
{
	typedef OrderedHash<String, shared_ptr<Table> > Tables;
	CWT_DENY_COPY(Scheme);
public:
	Scheme () : m_dbh(nullptr) {}
	Scheme (const String & uri) : m_dbh(nullptr) { open(uri); }

	void close ();
	bool open (const String & uri);
	bool opened () const { return m_dbh == nullptr ? false : m_dbh->opened(); }

	Table * addTable (const String & name);

	// bool populate(); // populate from scheme
	bool deploy ();
	bool drop ();

private:
	DbHandler * m_dbh;
	Tables      m_tables;
};

} // namespace debby

CWT_NS_END

#endif /* __CWT_DEBBY_SCHEME_HPP__ */

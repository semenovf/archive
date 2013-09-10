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
#include <cwt/hash.hpp>
#include <cwt/debby/dbh.hpp>

/*  Usage:

	Scheme scheme;
	Table table_0 = scheme.addTable("TableName0");
	Table table_1 = scheme.addTable("TableName1");
	...
	Table table_N = scheme.addTable("TableNameN");

	if (!scheme.deploy("sqlite://uri")) {
		// Error
	}

	...

	scheme = Scheme::open("sqlite://uri");
	Table table_M = scheme.addTable("TableNameM");

	...

	scheme = Debby::scheme(String("sqlite://uri"));
	scheme.delete();

 */

CWT_NS_BEGIN

class DLL_API Scheme
{
public:
	Scheme() : m_d(new SharedData) {}
	Scheme(const String & uri);

	Scheme(const Scheme & other) : m_d(other.m_d) {}
	Scheme & operator = (const Scheme & other);

	bool opened() const { return m_d->dbh != nullptr ? true : false; }
	bool deploy(const String & uri);

private:
	struct SharedData
	{
		DbHandler * dbh;

		SharedData  () : dbh(nullptr) {}
		~SharedData () {
			if (dbh) {
				dbh->close();
				delete dbh;
				dbh = nullptr;
			}
		}
		SharedData  (const SharedData & other) : dbh(other.dbh) {}
	};

	shareable<SharedData> m_d;
};

inline Scheme & Scheme::operator = (const Scheme & other)
{
	m_d = other.m_d;
	return *this;
}

CWT_NS_END

#endif /* __CWT_DEBBY_SCHEME_HPP__ */

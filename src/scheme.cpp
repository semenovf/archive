/*
 * scheme.cpp
 *
 *  Created on: Sep 10, 2013
 *      Author: wladt
 */

#include "../include/cwt/debby/scheme.hpp"
#include <cwt/logger.hpp>

/*
 * Example see in cwt-debby-sqlite3/tests/scheme.cpp.
 */

CWT_NS_BEGIN

/**
 * @brief Open scheme (database).
 *
 * @details Open scheme (database) specified by URI string.
 *
 * @param uri Scheme (database) uniform resource identifier.
 * @return @c false if scheme already opened or if an error occurred
 *            while opening the scheme, otherwise @c true.
 */
bool DebbyScheme::open (const String & uri)
{
	if (m_dbh) {
		Logger::error("Scheme already opened, close it before reopen.");
		return false;
	}
	m_dbh = DbHandler::open(uri);
	return m_dbh ? true : false;
}

/**
 *  @brief Close scheme.
 */
void DebbyScheme::close ()
{
	if (m_dbh) {
		delete m_dbh;    // automatically closed
		m_dbh = nullptr;
	}
}

/**
 * @fn bool Scheme::opened () const
 *
 * @brief Checks if the scheme (database) already opened.
 *
 * @return @c true if scheme (database) already opened, otherwise @c false.
 */


/**
 * @brief Drop scheme.
 *
 * @param name Scheme (database) name.
 * @return @c true if scheme (database) successfully dropped, otherwise @c false.
 */
bool DebbyScheme::drop ()
{
	bool r = true;
	if (m_dbh) {
		r = DbHandler::dropScheme(m_dbh);
		if (r) {
			delete m_dbh;
			m_dbh = nullptr;
		}
	}
	return r;
}


DebbyTable * DebbyScheme::addTable (const String & name)
{
	DebbyTables::iterator it = m_tables.insert(name, shared_ptr<DebbyTable>(new DebbyTable()));
	CWT_ASSERT(it != m_tables.end());
	DebbyTable * r = it.value().get();
	CWT_ASSERT(r);
	return r;
}

bool DebbyScheme::deploy ()
{
	return true;
}

CWT_NS_END



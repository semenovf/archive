/*
 * dbs.cpp
 *
 *  Created on: Aug 20, 2013
 *      Author: wladt
 */


//#include "../include/cwt/dbh.hpp"
#include "../include/cwt/debby/sth.hpp"

CWT_NS_BEGIN

/**
 *
 */
void DbStatement::close ()
{
	if (m_sth) {
		m_sth->driver->closeStmt(m_sth);
		m_sth = nullptr;
	}
}


/**
 * @fn Vector<UniType> DbStatement::fetchRow()
 *
 * @param sth
 * @return
 *
 * @note On error and at end of data returns empty vector, so need to call DbHandler::errno method to distinguishes this situation
 */


/**
 *
 * @param param
 * @return
 */
DbStatement & DbStatement::bind (const UniType & param)
{
	if (m_sth->driver->bind(*m_sth, m_bindCursor, param)) {
		++m_bindCursor;
	}
	return *this;
}

CWT_NS_END


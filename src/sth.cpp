/*
 * dbs.cpp
 *
 *  Created on: Aug 20, 2013
 *      Author: wladt
 */


//#include "../include/cwt/dbh.hpp"
#include "../include/cwt/sth.hpp"

CWT_NS_BEGIN

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


CWT_NS_END


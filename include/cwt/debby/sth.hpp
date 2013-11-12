/*
 * sth.hpp
 *
 *  Created on: Aug 19, 2013
 *      Author: wladt
 */

#ifndef __CWT_DEBBY_STH_HPP__
#define __CWT_DEBBY_STH_HPP__

#include <cwt/debby/dbd.hpp>

CWT_NS_BEGIN

namespace debby
{

class DbHandler;
class Statement;

typedef cwt::shared_ptr<Statement> StatementPtr;

class Statement
{
	friend class DbHandler;

public:
	Statement() : m_sth(nullptr), m_bindCursor(0) {}
	~Statement() { close(); }

	void        close();
	bool        exec ()         { m_bindCursor = 0; return m_sth->driver->execStmt(*m_sth); }
	bool        fetchRowArray(cwt::Vector<cwt::UniType> & row) { return m_sth->driver->fetchRowArray(*m_sth, row); }
	bool        fetchRowHash (cwt::Map<cwt::String, cwt::UniType> & row) { return m_sth->driver->fetchRowHash(*m_sth, row); }
	Statement & bind (const cwt::UniType & param);
	Statement & bind (size_t index, const cwt::UniType & param) { m_sth->driver->bind(*m_sth, index, param); return *this; }

private:
	DbStatementData * m_sth;
	size_t            m_bindCursor; // current bind index
};

} // namespace debby

CWT_NS_END

#endif /* __CWT_DEBBY_STH_HPP__ */

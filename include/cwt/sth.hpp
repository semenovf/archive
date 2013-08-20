/*
 * sth.hpp
 *
 *  Created on: Aug 19, 2013
 *      Author: wladt
 */

#ifndef __CWT_STH_HPP__
#define __CWT_STH_HPP__

#include <cwt/unitype.hpp>
#include <cwt/vector.hpp>
#include <cwt/dbd.hpp>

CWT_NS_BEGIN

class DbHandler;

class DbStatement
{
	friend class DbHandler;

public:
	DbStatement() : m_sth(nullptr) {}
	~DbStatement() { close(); }

	void            close();
	bool            exec ()         { return m_sth->driver->execStmt(*m_sth); }
	Vector<UniType> fetchRowArray() { return m_sth->driver->fetchRowArray(*m_sth); }
	Hash<String, UniType> fetchRowHash () { return m_sth->driver->fetchRowHash(*m_sth); }
/*
	ulong_t         lastId ();
	long_t          errno  ();
	String          strerror ();
	bool            bindByIndex (size_t index, const UniType & ut);
	size_t          bindParmsCount ();
	bool            isBindParmBounds (int index);
	bool            setParm (const UniType & ut, const void * copy, size_t sz);
	ulong_t         rows ();
	ulong_t         size ();
*/

private:
	DbStatementData * m_sth;
	//Vector<UniType>       bind_params;
};

CWT_NS_END


#endif /* __CWT_STH_HPP__ */

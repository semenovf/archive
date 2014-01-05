/*
 * sth.hpp
 *
 *  Created on: Aug 19, 2013
 *      Author: wladt
 */

#ifndef __CWT_DEBBY_STH_HPP__
#define __CWT_DEBBY_STH_HPP__

#include <cwt/debby/dbd.hpp>

namespace cwt { namespace debby {

class handler;

class statement
{
	friend class handler;
	PFS_PIMPL_INLINE(statement, statement_data);

protected:
	statement (statement_data * p) : _pimpl(p) {}
	statement () { PFS_ASSERT(true == false); }

public:
	~statement() { close(); }

	void close ();

	bool exec  ()
		{ _pimpl->_bindCursor = 0; return _pimpl->_driver->execStmt(*_pimpl); }
	bool fetchRowArray (pfs::vector<pfs::unitype> & row)
		{ return _pimpl->_driver->fetchRowArray(*_pimpl, row); }
	bool fetchRowHash (pfs::map<pfs::string, pfs::unitype> & row)
		{ return _pimpl->_driver->fetchRowHash(*_pimpl, row); }

	statement & bind (const pfs::unitype & param);
	statement & bind (size_t index, const pfs::unitype & param)
		{ _pimpl->_driver->bind(*_pimpl, index, param); return *this; }
};

}} // cwt::debby

#endif /* __CWT_DEBBY_STH_HPP__ */

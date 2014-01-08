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

	pfs::shared_ptr<statement_data> _pimpl;

protected:
	struct impl_deleter {
		void operator () (statement_data * p) const {
			if (p && p->_driver) {
				p->_driver->closeStmt(p);
			}
		}
	};

	statement (statement_data * p) : _pimpl(p, impl_deleter()) {}
	statement () { PFS_ASSERT(true == false); }

public:
	~statement() { close(); }

	void close ();
	bool exec  ();
	bool fetchRowArray (pfs::vector<pfs::unitype> & row);
	bool fetchRowHash (pfs::map<pfs::string, pfs::unitype> & row);
	statement & bind (const pfs::unitype & param);
	statement & bind (size_t index, const pfs::unitype & param);
};

}} // cwt::debby

#endif /* __CWT_DEBBY_STH_HPP__ */

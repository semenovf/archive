/*
 * sth.hpp
 *
 *  Created on: Aug 19, 2013
 *      Author: wladt
 */

#ifndef __CWT_DEBBY_STH_HPP__
#define __CWT_DEBBY_STH_HPP__

#include <cwt/debby/dbd.hpp>
#include <cwt/errorable.hpp>

namespace cwt { namespace debby {

class handler;

class statement : public errorable
{
	friend class handler;

	pfs::shared_ptr<statement_data> _pimpl;

protected:
	struct impl_deleter {
		void operator () (statement_data * p) const
		{
			if (p && p->_driver) {
				p->_driver->closeStmt(p);
				p->_driver = nullptr;
			}
		}
	};

	statement (statement_data * p) : errorable(), _pimpl(p, impl_deleter()) {}
	statement () : errorable(), _pimpl() { PFS_ASSERT(true == false); }

public:
	statement (const statement & other) : errorable(), _pimpl(other._pimpl) {}
	statement & operator = (const statement & other)
	{
		_pimpl = other._pimpl;
		return *this;
	}
	~statement () { /*close();*/ }

	void close ();
	bool exec  ();
	pfs::vector<pfs::unitype> fetchRowArray ();
	pfs::map<pfs::string, pfs::unitype> fetchRowHash ();
	statement & bind (const pfs::unitype & param);
	statement & bind (size_t index, const pfs::unitype & param);
};

}} // cwt::debby

#endif /* __CWT_DEBBY_STH_HPP__ */

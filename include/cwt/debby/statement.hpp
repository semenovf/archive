/*
 * statement.hpp
 *
 *  Created on: Aug 19, 2013
 *      Author: wladt
 */

#ifndef __CWT_DEBBY_STATEMENT_HPP__
#define __CWT_DEBBY_STATEMENT_HPP__

#include <cwt/debby/dbd.hpp>
#include <cwt/errorable.hpp>

namespace cwt { namespace debby {

class database;

class statement : public errorable
{
	friend class database;

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
	statement () : errorable(), _pimpl() {}

public:
	statement (const statement & other) : errorable(), _pimpl(other._pimpl) {}
	statement & operator = (const statement & other)
	{
		_pimpl = other._pimpl;
		return *this;
	}
	~statement () { /*close();*/ }

	bool isNull () const { return !(_pimpl && _pimpl->_driver); }
	void close ();
	bool exec  ();

	ulong_t  rows ()       { return _pimpl->_driver->rows(*_pimpl); }
	long_t   lastId ()     { return _pimpl->_driver->lastId(*_pimpl); }

	pfs::vector<pfs::unitype> fetchRowArray ();
	pfs::map<pfs::string, pfs::unitype> fetchRowHash ();
	statement & bind (const pfs::unitype & param);
	statement & bind (size_t index, const pfs::unitype & param);
	size_t      columnCount ();
	pfs::string columnName (size_t index);
	column_type columnType (size_t index);

};

}} // cwt::debby

#endif /* __CWT_DEBBY_STATEMENT_HPP__ */

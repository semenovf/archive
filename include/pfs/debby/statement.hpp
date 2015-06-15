/*
 * statement.hpp
 *
 *  Created on: Aug 19, 2013
 *      Author: wladt
 */

#ifndef __PFS_DEBBY_STATEMENT_HPP__
#define __PFS_DEBBY_STATEMENT_HPP__

#include <pfs/debby/dbd.hpp>
#include <pfs/errorable.hpp>

namespace pfs { namespace debby {

class database;

class statement : public errorable
{
	friend class database;

	shared_ptr<statement_data> _pimpl;

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

	uintegral_t  rows ()       { return _pimpl->_driver->rows(*_pimpl); }
	integral_t   lastId ()     { return _pimpl->_driver->lastId(*_pimpl); }

	vector<unitype> fetchRowArray ();
	map<string, unitype> fetchRowHash ();
	statement & bind (const unitype & param);
	statement & bind (size_t index, const unitype & param);
	size_t      columnCount ();
	string      columnName (size_t index);
	column_type columnType (size_t index);

};

}} // pfs::debby

#endif /* __PFS_DEBBY_STATEMENT_HPP__ */

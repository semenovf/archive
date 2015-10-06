/*
 * statement.hpp
 *
 *  Created on: Aug 19, 2013
 *      Author: wladt
 */

#ifndef __PFS_DEBBY_STATEMENT_HPP__
#define __PFS_DEBBY_STATEMENT_HPP__

#include <pfs/errorable.hpp>
#include <pfs/noncopyable.hpp>

namespace pfs { namespace debby {

class database;

class statement : public errorable, noncopyable
{
//	friend class database;
//
//	shared_ptr<statement_data> _pimpl;

protected:
//	struct impl_deleter {
//		void operator () (statement_data * p) const
//		{
//			if (p && p->_driver) {
//				p->_driver->closeStmt(p);
//				p->_driver = nullptr;
//			}
//		}
//	};

public:
//	statement (const statement & other) : errorable(), _pimpl(other._pimpl) {}
//	statement & operator = (const statement & other)
//	{
//		_pimpl = other._pimpl;
//		return *this;
//	}
	virtual ~statement ();// { /*close();*/ }

	virtual bool isNull () const = 0; //{ return !(_pimpl && _pimpl->_driver); }
	virtual void close () = 0;
	virtual bool exec  () = 0;

	virtual uintegral_t rows () = 0; //       { return _pimpl->_driver->rows(*_pimpl); }
	virtual integral_t  lastId () = 0; //    { return _pimpl->_driver->lastId(*_pimpl); }

	virtual vector<variant_type> fetchRowArray () = 0;
	virtual map<string, variant_type> fetchRowHash () = 0;
	virtual statement & bind (const variant_type & param) = 0;
	virtual statement & bind (size_t index, const variant_type & param) = 0;
	virtual size_t      columnCount () = 0;
	virtual string      columnName (size_t index) = 0;
	virtual column_type columnType (size_t index) = 0;
};

}} // pfs::debby

#endif /* __PFS_DEBBY_STATEMENT_HPP__ */

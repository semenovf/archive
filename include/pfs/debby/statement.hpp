/*
 * statement.hpp
 *
 *  Created on: Aug 19, 2013
 *      Author: wladt
 */

#ifndef __PFS_DEBBY_STATEMENT_HPP__
#define __PFS_DEBBY_STATEMENT_HPP__

#include <pfs/errorable.hpp>
#include <pfs/map.hpp>
#include <pfs/vector.hpp>
#include "dbd.hpp"

namespace pfs { namespace debby {

class database;
class statement_impl;

class statement : public pfs::nullable<statement_impl>, public errorable_ext
{
//	friend class database;
private:
	statement (const statement & other);
//		: _d(other._d)
//	{}

	statement & operator = (const statement & other);
//	{
//		_d = other._d;
//		return *this;
//	}

public:
	virtual ~statement ();// { /*close();*/ }

	void close ();
	bool exec ();

	uintegral_t rows () const;
	integral_t  lastId () const;

	vector<variant_type> fetchRowArray ();
	map<string, variant_type> fetchRowHash ();
	statement & bind (const variant_type & param);
	statement & bind (size_t index, const variant_type & param);
	size_t columnCount () const;
	string columnName (size_t index) const;
	column_type columnType (size_t index) const;
};

}} // pfs::debby

#endif /* __PFS_DEBBY_STATEMENT_HPP__ */

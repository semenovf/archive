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
#include <pfs/debby/dbd.hpp>

namespace pfs { namespace debby {

class database;
class statement_impl;

class statement
{
//	friend class database;
	statement_impl * _d;

private:
	statement (const statement & other);
	statement & operator = (const statement & other);

public:
	virtual ~statement ();

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

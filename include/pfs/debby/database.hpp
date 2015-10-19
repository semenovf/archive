/*
 * database.hpp
 *
 *  Created on: Aug 19, 2013
 *      Author: wladt
 */

#ifndef __PFS_DEBBY_DATABASE_HPP__
#define __PFS_DEBBY_DATABASE_HPP__

#include <pfs/stringlist.hpp>
#include <pfs/errorable.hpp>
#include <pfs/vector.hpp>
#include <pfs/debby/dbd.hpp>

namespace pfs { namespace debby {

class statement;
class database_impl;

class database : public errorable
{
	database_impl * _d;
	string _pluginPrefix;

public:
	database ()	: _d(0) {}

	/**
	 */
	~database ();
//	{
//		close();
//		_d.reset();
//	}

	/**
	 * @brief
	 *
	 * @param uri
	 * @return
	 * @see database::open().
	 */
	bool connect (const string & uri)
	{
		return open(uri);
	}

	/**
	 * @brief Establish connection to the database specified by URI.
	 * @param uri URI for connection
	 * @return @c true if connection to the database has been established,
	 *         @c false otherwise.
	 */
	bool open (const string & uri);

	/**
	 * @brief Checks if if connection to the database has been established.
	 *
	 * @return @c true if connection to the database has been established before,
	 *         @c false otherwise.
	 */
	bool connected () const
	{
		return opened();
	}

	bool opened () const
	{
		return _d != 0;
	}


	/**
	 * @brief Close connection to the database if it was established before.
	 */
	void close ();
//	{
//		if (opened())
//			base_class::cast()->close();
//	}

	/**
	 *
	 * @param sql
	 * @return
	 *
	 * @note Cannot be used for statements that contain binary data
	 */
	bool query (const string & sql);
	statement prepare (const string & sql);
	stringlist tables () const;
	bool tableExists (const string & name) const;
	bool setAutoCommit (bool on);
	bool autoCommit () const;
	bool begin ();
	bool commit ();
	bool rollback ();
	bool end (bool success)
	{
		return success ? commit() : rollback();
	}

	integral_t errorCode ();
//	{
//		return _pdd->_driver->errorCode(*_pdd);
//	}

	vector<column_meta> meta (const string & table);
};

}} // pfs::debby

#endif /* __PFS_DEBBY_DATABASE_HPP__ */

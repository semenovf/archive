/*
 * database.hpp
 *
 *  Created on: Aug 19, 2013
 *      Author: wladt
 */

#ifndef __PFS_DEBBY_DATABASE_HPP__
#define __PFS_DEBBY_DATABASE_HPP__

#include <pfs/debby/dbd.hpp>
#include <pfs/vector.hpp>
#include <pfs/shared_ptr.hpp>
#include <pfs/errorable_ext.hpp>
#include <pfs/noncopyable.hpp>

namespace pfs { namespace debby {

class statement;

class database : public errorable_ext, noncopyable
{
	shared_ptr<database_data> _pdd;

public:
	database ()
		: _pdd()
	{}

	database (const string & uri)
		: _pdd()
	{
		open(uri);
	}

	database (const char * uri)
		: _pdd()
	{
		open(_l1(uri));
	}

	~database () { /*close();*/ }

	bool isNull () const
	{
		return !(_pdd && _pdd->_driver);
	}

	bool open (const string & uri);

	bool opened () const
	{
		return _pdd && _pdd->_driver != nullptr;
	}

	void close ();

	/**
	 *
	 * @param sql
	 * @return
	 *
	 * @note Cannot be used for statements that contain binary data
	 */
	bool query (const string & sql);
	statement prepare (const string & sql);

	vector<string> tables ()
	{
		return _pdd->_driver->tables(*_pdd);
	}

	bool tableExists (const string & name)
	{
		return _pdd->_driver->tableExists(*_pdd, name);
	}

	bool setAutoCommit (bool on)
	{
		return _pdd->_driver->setAutoCommit(*_pdd, on);
	}

	bool autoCommit ()
	{
		return _pdd->_driver->autoCommit(*_pdd);
	}

	bool begin ();
	bool commit ();
	bool rollback ();

	bool end (bool success)
	{
		return success ? commit() : rollback();
	}

	integral_t errorCode ()
	{
		return _pdd->_driver->errorCode(*_pdd);
	}

	bool meta (const string & table, pfs::vector<column_meta> & meta);
};

}} // pfs::debby

#endif /* __PFS_DEBBY_DATABASE_HPP__ */

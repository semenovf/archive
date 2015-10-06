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
#include <pfs/pimpl.hpp>
#include <pfs/errorable_ext.hpp>

namespace pfs { namespace debby {

class debby;
class statement;
class database_impl;

class database : public pfs::nullable<database_impl>, public errorable_ext
{
	friend class debby;

	pimpl _d;
//	shared_ptr<database_data> _pdd;
protected:
	database (const database & other)
		: _d(other._d)
	{}

	database & operator = (const database & other)
	{
		_d = other._d;
		return *this;
	}

public:
//	database ()
//		: _pdd()
//	{}
//
//	database (const string & uri)
//		: _pdd()
//	{
//		open(uri);
//	}
//
//	database (const char * uri)
//		: _pdd()
//	{
//		open(_l1(uri));
//	}

	~database (); // { /*close();*/ }

	virtual bool open (const string & uri) = 0;

	virtual bool opened () const = 0;
//	{
//		return _pdd && _pdd->_driver != nullptr;
//	}

	virtual void close () = 0;

	/**
	 *
	 * @param sql
	 * @return
	 *
	 * @note Cannot be used for statements that contain binary data
	 */
	virtual bool query (const string & sql) = 0;
	virtual statement * prepare (const string & sql) = 0;

	virtual vector<string> tables () = 0
//	{
//		return _pdd->_driver->tables(*_pdd);
//	}

	virtual bool tableExists (const string & name) = 0;
//	{
//		return _pdd->_driver->tableExists(*_pdd, name);
//	}

	virtual bool setAutoCommit (bool on) = 0;
//	{
//		return _pdd->_driver->setAutoCommit(*_pdd, on);
//	}

	virtual bool autoCommit () = 0;
//	{
//		return _pdd->_driver->autoCommit(*_pdd);
//	}

	virtual bool begin () = 0;
	virtual bool commit () = 0;
	virtual bool rollback () = 0;
	virtual bool end (bool success) = 0;
//	{
//		return success ? commit() : rollback();
//	}

	virtual integral_t errorCode () = 0;
//	{
//		return _pdd->_driver->errorCode(*_pdd);
//	}

	virtual bool meta (const string & table, pfs::vector<column_meta> & meta) = 0;
};

}} // pfs::debby

#endif /* __PFS_DEBBY_DATABASE_HPP__ */

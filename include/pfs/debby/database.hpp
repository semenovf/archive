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
#include <pfs/dl.hpp>

namespace pfs { namespace debby {

class statement;

class database : public pfs::dl
{
	shared_ptr<database_data> _pdd;

protected:
//	struct impl_deleter {
//		void operator () (database_data * p) const
//		{
//			if (p && p->_driver) {
//				p->_driver->close(p);
//				p->_driver = nullptr;
//			}
//		}
//	};

	//database (handler_data * p) : _pimpl(p, impl_deleter()) {}

public:
	database () : pfs::dl(), _pdd() {}
	database (const string & uri) : pfs::dl(), _pdd() { open(uri); }
	database (const char * uri) : pfs::dl(), _pdd() { open(_l1(uri)); }
	database (const database & other) : pfs::dl(), _pdd(other._pdd) {}
	~database () { /*close();*/ }

	bool isNull () const { return !(_pdd && _pdd->_driver); }

	bool open (const pfs::string & uri);
	bool opened () const { return _pdd && _pdd->_driver != nullptr; }
	void close ();

	bool query (const pfs::string & sql); // cannot be used for statements that contain binary data
	statement prepare (const pfs::string & sql);

	vector<string>
		     tables ()    { return _pdd->_driver->tables(*_pdd); }

	bool     tableExists   (const pfs::string & name) { return _pdd->_driver->tableExists(*_pdd, name); }
	bool     setAutoCommit (bool on) { return _pdd->_driver->setAutoCommit(*_pdd, on); }
	bool     autoCommit () { return _pdd->_driver->autoCommit(*_pdd); }
	bool     begin ();
	bool     commit ();
	bool     rollback ();
	bool     end (bool success) { return success ? commit() : rollback(); }
	integral_t errorCode ()      { return _pdd->_driver->errorCode(*_pdd); }

	bool     meta (const pfs::string & table, pfs::vector<column_meta> & meta);
};

}} // pfs::debby

#endif /* __PFS_DEBBY_DATABASE_HPP__ */

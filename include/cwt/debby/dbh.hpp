/*
 * dbh.hpp
 *
 *  Created on: Aug 19, 2013
 *      Author: wladt
 */

#ifndef __CWT_DEBBY_DBH_HPP__
#define __CWT_DEBBY_DBH_HPP__

#include <cwt/debby/dbd.hpp>
#include <pfs/vector.hpp>
#include <pfs/pimpl.hpp>
#include <cwt/errorable.hpp>

namespace cwt { namespace debby {

class statement;
class handler;

class handler : public errorable
{
	pfs::shared_ptr<handler_data> _pimpl;

protected:
	struct impl_deleter {
		void operator () (handler_data * p) const
		{
			if (p && p->_driver) {
				p->_driver->close(p);
				p->_driver = nullptr;
			}
		}
	};

	//handler (handler_data * p) : _pimpl(p, impl_deleter()) {}

public:
	handler () : _pimpl() {}
	handler (const handler & other) : _pimpl(other._pimpl) {}

	~handler () { /*close();*/ }

	bool open (const pfs::string & uri);
	bool opened () const { return _pimpl && _pimpl->_driver != nullptr; }
	void close ();

	bool query (const pfs::string & sql); // cannot be used for statements that contain binary data
	statement prepare (const pfs::string & sql);

	ulong_t  rows ()       { return _pimpl->_driver->rows(*_pimpl); }
	ulong_t  lastId ()     { return _pimpl->_driver->lastId(*_pimpl); }

	pfs::vector<pfs::string>
		     tables ()    { return _pimpl->_driver->tables(*_pimpl); }

	bool     tableExists   (const pfs::string & name) { return _pimpl->_driver->tableExists(*_pimpl, name); }
	bool     setAutoCommit (bool on) { return _pimpl->_driver->setAutoCommit(*_pimpl, on); }
	bool     autoCommit () { return _pimpl->_driver->autoCommit(*_pimpl); }
	bool     begin ();
	bool     commit ();
	bool     rollback ();
	long_t   errno ()      { return _pimpl->_driver->errno(*_pimpl); }

	bool     meta (const pfs::string & table, pfs::vector<column_meta> & meta);
};

}} // cwt::debby

#endif /* __CWT_DEBBY_DBH_HPP__ */

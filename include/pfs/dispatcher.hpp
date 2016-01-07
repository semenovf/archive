/*
 * dispatcher.hpp
 *
 *  Created on: Feb 9, 2011
 *      Author: wladt
 *  Removed to CWT on Feb 12, 2013
 *  Renamed to dispatcher Aug 25, 2015
 */

#ifndef __PFS_DISPATCHER_HPP__
#define __PFS_DISPATCHER_HPP__

#include <pfs/map.hpp>
#include <pfs/vector.hpp>
#include <pfs/fs/path.hpp>
#include <pfs/mutex.hpp>
#include <pfs/thread.hpp>
#include <pfs/dl.hpp>
#include <pfs/sigslotmapping.hpp>
#include <pfs/notification.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

struct module_spec
{
	module * mod;
	dynamic_library dl; /* null for local */
	module_dtor_t dtor;  /* may be null (no destructor) */

	module_spec ()
		: mod(0)
		, dl(0)
		, dtor(0)
	{}

	module_spec (module * a
			, const dynamic_library & b
			, module_dtor_t c = default_dtor)
		: mod(a)
		, dl(b)
		, dtor(c) {}

	static void default_dtor (module *);
};

class DLL_API dispatcher : public has_slots<>
{
public:
	typedef struct { int id; sigslot_mapping_t * map; string desc; } mapping_type;
	typedef map<int, mapping_type *> mapping_collection;
	typedef map<string, module_spec> module_spec_map;

private:
	fs::pathlist       _searchdirs;
	mapping_collection _mapping;
	module_spec_map    _modules;
	vector<thread *>   _threads;
	module *           _master_module;
	notification       _nx;

private:
	dispatcher (const dispatcher &);
	dispatcher & operator = (const dispatcher &);

protected:
    dispatcher ()
		: _master_module(0)
	{}

public:
	dispatcher (mapping_type * mapping, int n);

	virtual ~dispatcher ();

	const notification & get_notification () const
	{
		return _nx;
	}

	notification & get_notification ()
	{
		return _nx;
	}

	void add_search_path (const fs::path & dir)
	{
		if (!dir.empty())
			_searchdirs.append(dir);
	}

	module * register_local_module (module * mod);

	module * register_module_for_path (const fs::path & path
			, const char * class_name = 0
			, int argc = 0
			, const char ** argv = 0);

	module * register_module_for_name (const string & name
			, const char * class_name = 0
			, int argc = 0
			, const char ** argv = 0);

	void set_master_module (module * mod)
	{
		_master_module = mod;
	}

	size_t count () const
	{
		return _modules.size();
	}

/* TODO need implementation
	bool registerModuleForUrl(const string &url);
*/
	void connect_all ();
	void disconnect_all ();
	void unregister_all ();
	bool start ();
	int  exec ();

	bool is_module_registered (const string & pname)
	{
		return _modules.contains(pname);
	}

public: /*slots*/
	void on_module_registered (const string & pname, bool & result)
	{
		result = is_module_registered(pname);
	}

protected:
	bool register_module (module & m, dynamic_library::handle ph, module_dtor_t dtor);
};

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_DISPATCHER_HPP__ */

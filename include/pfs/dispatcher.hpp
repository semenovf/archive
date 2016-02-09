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
#include <pfs/shared_ptr.hpp>
#include <pfs/sigslotmapping.hpp>
#include <pfs/utility.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

struct module_spec
{
	shared_ptr<module> pmodule;
	dynamic_library dl;

	~module_spec ()
	{
		// Need to destroy pmodule before dynamic library will be destroyed automatically
		//
		pmodule.reset();
	}
};

static void default_dtor (module *);

class DLL_API dispatcher : public has_slots<>
{
public:
	typedef struct api_item_type_t
	{
		int id;
		sigslot_mapping_t * map;
		string desc;
	} api_item_type;

	typedef map<int, api_item_type *> api_type;
	typedef map<string, module_spec> module_spec_map_type;

private:
	fs::pathlist         _searchdirs;
	api_type             _api;
	module_spec_map_type _module_spec_map;
	vector<thread *>     _threads;
	shared_ptr<module>   _master_module;

private:
	dispatcher (const dispatcher &);
	dispatcher & operator = (const dispatcher &);

protected:
    dispatcher ()
		: _master_module(0)
	{}

public:
	dispatcher (api_item_type * mapping, int n);

	virtual ~dispatcher ()
	{
		finalize();
	}

	void finalize ();

	void add_search_path (const fs::path & dir)
	{
		if (!dir.empty())
			_searchdirs.push_back(dir);
	}

	bool register_local_module (module * pmodule, const string & name)
	{
		module_spec modspec;
		modspec.pmodule = shared_ptr<module>(pmodule);
		modspec.dl = dynamic_library();
		modspec.pmodule->set_name(name);
		return register_module(modspec);
	}

	bool register_module_for_path (const fs::path & path
			, const string & name
			, const char * class_name = 0
			, void * mod_data = 0)
	{
		module_spec modspec = module_for_path(path, class_name, mod_data);
		modspec.pmodule->set_name(name);
		return register_module(modspec);
	}

	bool register_module_for_name (const string & name
			, const char * class_name = 0
			, void * mod_data = 0)
	{
		module_spec modspec = module_for_name(name, class_name, mod_data);
		if (modspec.pmodule) {
			modspec.pmodule->set_name(name);
			return register_module(modspec);
		}
		return false;
	}

	void set_master_module (const string & name);

	size_t count () const
	{
		return _module_spec_map.size();
	}

/* TODO need implementation
	bool registerModuleForUrl(const string &url);
*/
	void connect_all ();
	void disconnect_all ();
	void unregister_all ();
	bool start ();
	int  exec ();

	bool is_module_registered (const string & modname) const
	{
		return _module_spec_map.find(modname) != _module_spec_map.end();
	}

public: // signals
	signal0<> emit_quit;

public: // slots
	void module_registered (const string & pname, bool & result)
	{
		result = is_module_registered(pname);
	}

	void broadcast_quit ()
	{
		emit_quit();
	}

	void print_info  (const string & s);
	void print_debug (const string & s);
	void print_warn  (const string & s);
	void print_error (const string & s);

protected:
	module_spec module_for_path (const fs::path & path
			, const char * class_name = 0
			, void * mod_data = 0);

	module_spec module_for_name (const string & name
			, const char * class_name = 0
			, void * mod_data = 0)
	{
		fs::path modpath = dynamic_library::build_filename(name);
		return module_for_path(modpath, class_name, mod_data);
	}

	bool register_module (const module_spec & modspec);
};

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_DISPATCHER_HPP__ */

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
#include <pfs/noncopyable.hpp>
#include <pfs/dl.hpp>
#include <pfs/mutex.hpp>
#include <pfs/thread.hpp>
#include <pfs/sigslotmapping.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

struct module_spec
{
	module_spec () : mod(nullptr), ph(nullptr), dtor(nullptr) {}
	module_spec (module * a, dl::handle b, module_dtor_t c) : mod(a), ph(b), dtor(c) {}
	module * mod;
	dl::handle ph;        /* null for local */
	module_dtor_t dtor;   /* may be null (no destructor) */
};

class DLL_API dispatcher : public errorable_ext, public has_slots<>, noncopyable
{
public:
	typedef struct { int id; sigslot_mapping_t * map; string desc; } mapping_type;
	typedef map<int, mapping_type *> mapping_collection_type;
	typedef map<string, module_spec> module_specs_type;

private:
	mapping_collection_type _mapping;
	module_specs_type _modules;
	vector<thread *>  _threads;
	module *          _masterModule;

protected:
    dispatcher() : _masterModule(nullptr) {}

public:
	dispatcher (mapping_type mapping[], int n);
	virtual ~dispatcher() {
	    disconnectAll();
	    unregisterAll();
	}

	void addSearchPath (const string & dir);

	module * registerLocalModule (module * mod, module_dtor_t dtor = module::defaultDtor);
	module * registerModuleForPath (const string & path, const char * modname = nullptr, int argc = 0, const char ** argv = nullptr);
	module * registerModuleForName (const string & name, const char * modname = nullptr, int argc = 0, const char ** argv = nullptr);
	void setMasterModule (module * mod) { _masterModule = mod; }
	size_t count () const  { return _modules.size(); }

/* TODO need implementation
	bool registerModuleForUrl(const string &url);
*/
	void connectAll ();
	void disconnectAll ();
	void unregisterAll ();
	bool start ();
	int  exec ();

	bool isModuleRegistered (const string & pname)
		{ return _modules.contains(pname); }

public: /*slots*/
	void onModuleRegistered (const string & pname, bool & result)
	{
		result = isModuleRegistered(pname);
	}

protected:
	bool registerModule (module & m, dl::handle ph, module_dtor_t dtor);
};

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_DISPATCHER_HPP__ */

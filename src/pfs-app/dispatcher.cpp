/*
 * dispacther.cpp
 *
 *  Created on: Feb 9, 2011
 *      Author: wladt
 *  Removed to CWT on: Feb 12, 2013
 */

#include "pfs/dispatcher.hpp"

#include <pfs/dl.hpp>
#include "pfs/safeformat.hpp"
#include "pfs/logger.hpp"
#include "pfs/fs.hpp"

namespace pfs {

class module_threaded : public thread
{
public:
	module_threaded (module * p) : thread() , _mod(p) { PFS_ASSERT(_mod); }
	virtual ~module_threaded () {}
	const module * modulePtr () const { return _mod; }

protected:
	virtual void run() { _mod->run(_mod); }

private:
	module * _mod;
};

static void __notify (pfs::notification & nx, pfs::notification_type_enum nxtype, const string & title, const string & body)
{
	string msg;
	msg.append(title);
	msg.append(": ");
	msg.append(body);
	nx.append(nxtype, msg);
}

static void __notify (pfs::notification & nx, pfs::notification_type_enum nxtype, const string & body)
{
	string msg;
	msg.append(body);
	nx.append(nxtype, msg);
}

inline void __notify_error (pfs::notification & nx, const string & title, const string & body)
{
	__notify(nx, pfs::notification_error, title, body);
}

inline void __notify_warn (pfs::notification & nx, const string & title, const string & body)
{
	__notify(nx, pfs::notification_warn, title, body);
}

inline void __notify_warn (pfs::notification & nx, const string & body)
{
	__notify(nx, pfs::notification_warn, body);
}

inline void __notify_debug (pfs::notification & nx, const string & title, const string & body)
{
	__notify(nx, pfs::notification_debug, title, body);
}

dispatcher::dispatcher (mapping_type * mapping, int n)
	: _master_module(0)
{
	for (int i = 0; i < n; i++) {
		_mapping.insert(mapping_collection::value_type(mapping[i].id, & mapping[i]));
	}
}

dispatcher::~dispatcher ()
{
    disconnect_all();
    unregister_all();
    pfs::log::print(_nx);
    _nx.clear();
}

module * dispatcher::register_local_module (module * mod)
{
	if (mod)
		return register_module(*mod, 0, module_spec::default_dtor) ? mod : 0;
	return 0;
}

module * dispatcher::register_module_for_path (const fs::path & path
			, const string & modname
			, int arg
			, const char ** argv)
{
	module * result = 0;
	pfs::dynamic_library dl;
	error_code ex;
	string extended_errstr;

	if (!dl.open(path, _searchdirs, & ex, & extended_errstr)) {
		__notify_error(_nx, path.native(), to_string(ex));

		if (not extended_errstr.empty())
			__notify_error(_nx, path.native(), extended_errstr);

		return 0;
	}

	dynamic_library::symbol ctor = dl.resolve(PFS_MODULE_CTOR_NAME
			, & ex
			, & extended_errstr);

	if (ctor) {
		__notify_error(_nx, path.native(), _u8("Failed to resolve `ctor' for module"));
		__notify_error(_nx, path.native(), to_string(ex));

		if (not extended_errstr.empty())
			__notify_error(_nx, path.native(), extended_errstr);
	}

	dynamic_library::symbol dtor = dl.resolve(PFS_MODULE_DTOR_NAME
			, & ex
			, & extended_errstr);

	if (ctor) {
		__notify_error(_nx, path.native(), _u8("Failed to resolve `dtor' for module"));
		__notify_error(_nx, path.native(), to_string(ex));

		if (not extended_errstr.empty())
			__notify_error(_nx, path.native(), extended_errstr);
	}


	if (ctor && dtor) {
		module_ctor_t module_ctor = reinterpret_cast<module_ctor_t>(ctor);
		module_dtor_t module_dtor = reinterpret_cast<module_dtor_t>(dtor);

		if (module_ctor) {
			result = reinterpret_cast<module *>(module_ctor(modname.c_str(), arg, argv));

			if (result) {
				if (register_module(*mod, ph, module_dtor)) {
					;
				} else {
					result = 0; // Invalidate result;
				}
			}
		}
	}

	return 0;
}

module * dispatcher::register_module_for_name (const string & name, const char * modname, int arg, const char ** argv)
{
	pfs::dynamic_library & dl = pfs::dynamic_library::getDL();
	string filename = dl.buildDlFileName(name);
	string realPath;
	bool global = false;  // Avoid name conflicts
	bool resolve = true;
	dynamic_library::handle ph = dl.open(filename, & realPath, global, resolve); // try to find module

	if (ph) {
		dl.close(ph);
		string msg;
		msg.append(_u8("module found at "));
		msg.append(realPath);
		__notify_debug(_nx, name, msg);
		return register_module_for_path(realPath, modname, arg, argv);
	} else {
		// FIXME need dl to use notification instead of errorable_ext
		// and replace below code with
		// _nx.append(dl.get_notification())
		_nx.append(dl.lastErrorText());
	}

	__notify_error(_nx, name, _u8("module not found by specified name or may be inconsistent"));
	return 0;
}

bool dispatcher::register_module (module & mod, const dynamic_library ::handle ph, module_dtor_t dtor)
{
	int nemitters, ndetectors;

	if (_modules.find(mod._name) != _modules.cend()) {
		__notify_error(_nx, mod._name, _u8("module already registered"));
		return false;
	}

	const emitter_mapping * emitters = mod.get_emitters(& nemitters);
	const detector_mapping * detectors = mod.get_detectors(& ndetectors);

	mapping_collection::iterator it_end = _mapping.end();

	if (emitters) {
		for (int i = 0; i < nemitters; i++) {
			int emitter_id(emitters[i]._id);
			mapping_collection::iterator it = _mapping.find(emitter_id);
			if (it != it_end) {
				it.value()->map->appendEmitter(reinterpret_cast<emitter *>(emitters[i]._emitter));
			} else {
				string tmp;
				string msg;
				msg.append(": ");
				msg.append(_u8("emitter"));
				msg.append(" '");
				msg.append(lexical_cast(emitters[i]._id, tmp));
				msg.append("' ");
				msg.append(_u8("not found while registering module ..."));
				__notify_warn(_nx, mod.name(), msg);
				__notify_warn(_nx, _u8("... may be signal/slot mapping is not supported for this application"));
			}
		}
	}

	if (detectors) {
		for (int i = 0; i < ndetectors; i++) {
			int detector_id(detectors[i]._id);
			mapping_collection::iterator it = _mapping.find(detector_id);
			if (it != it_end) {
				it.value()->map->appendDetector(& mod, detectors[i]._detector);
			} else {
				string tmp;
				string msg;
				msg.append(_u8("detector"));
				msg.append(" '");
				msg.append(lexical_cast(emitters[i]._id, tmp));
				msg.append("' ");
				msg.append(_u8("not found while registering module ..."));
				__notify_warn(_nx, mod.name(), msg);
				__notify_warn(_nx, _u8("... may be signal/slot mapping is not supported for this application"));
			}
		}
	}

	mod.emit_module_registered.connect(this, & dispatcher::on_module_registered);
	mod._pdispatcher = this;
	module_spec pspec(& mod, ph, dtor);

	_modules.insert(mod._name, pspec);

	// module must be run in a separate thread.
	if (mod.run) {
		_threads.append(new module_threaded(& mod));
		__notify_debug(_nx, mod.name(), _u8("module registered as threaded"));
	} else {
		__notify_debug(_nx, mod.name(), _u8("module registered"));
	}

	return true;
}

void dispatcher::connect_all ()
{
	mapping_collection::iterator it = _mapping.begin();
	mapping_collection::iterator itEnd = _mapping.end();

	for(; it != itEnd; it++ ) {
		it.value()->map->connect_all();
	}
}

void dispatcher::disconnect_all ()
{
	mapping_collection::iterator it = _mapping.begin();
	mapping_collection::iterator itEnd = _mapping.end();

	for(; it != itEnd; it++ ) {
		it.value()->map->disconnect_all();
	}
}

void dispatcher::unregister_all ()
{
	vector<thread *>::iterator itThread = _threads.begin();
	vector<thread *>::iterator itThreadEnd = _threads.end();

	for (; itThread != itThreadEnd; ++itThread) {
		delete *itThread;
	}
	_threads.clear();

	module_spec_map::iterator it = _modules.begin();
	module_spec_map::iterator itEnd = _modules.end();

	for (;it != itEnd; it++) {
		module_spec pspec = it.value();
		module * mod = pspec.mod;
		PFS_ASSERT(mod);
		mod->emit_module_registered.disconnect(this);
		string pname = mod->name();

		if (pspec.dtor) {
			pspec.dtor(mod);
		}

		if (pspec.dl) {
			pfs::dynamic_library & dl = pfs::dynamic_library::getDL();
			dl.close(pspec.dl);
		}

		__notify_debug(_nx, pname, _u8("module unregistered"));
	}
	_modules.clear();
}

bool dispatcher::start ()
{
	bool r = true;

	module_spec_map::iterator it = _modules.begin();
	module_spec_map::iterator itEnd = _modules.end();

	for (;it != itEnd; it++) {
		module_spec pspec = it.value();
		module * mod = pspec.mod;
		PFS_ASSERT(mod);

		if (!mod->on_start(_nx)) {
			__notify_error(_nx, mod->name(), _u8("failed to start module"));
			r = false;
		}
	}

	pfs::log::print(_nx);
	_nx.clear();

	return r;
}

int dispatcher::exec ()
{
	int r = 0;

	// Exclude master module from dispatcher's threads pool
	if (_master_module) {
		size_t size = _threads.size();
		for (size_t i = 0; i < size; ++i) {
			const thread * th = _threads[i];
			const module_threaded * pt = dynamic_cast<const module_threaded*>(th);
			if (pt->modulePtr() == _master_module) {
				_threads.remove(i);
				delete pt;
				break;
			}
		}
	}

	vector<thread *>::iterator itThread = _threads.begin();
	vector<thread *>::iterator itThreadEnd = _threads.end();

	for (; itThread != itThreadEnd; ++itThread) {
		thread * t = *itThread;
		t->start();
	}

	if (_master_module && _master_module->run) {
		r = _master_module->run(_master_module);
	}

	for (itThread = _threads.begin(); itThread != itThreadEnd; ++itThread) {
		thread * t = *itThread;
		t->wait();
	}

	module_spec_map::iterator itModule = _modules.begin();
	module_spec_map::iterator itModuleEnd = _modules.end();

	for (; itModule != itModuleEnd; itModule++) {
		module_spec modspec = itModule.value();
		module * mod = modspec.mod;
		PFS_ASSERT(mod);
		mod->on_finish();
	}

	return r;
}

} // pfs

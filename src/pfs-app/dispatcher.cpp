/*
 * dispacther.cpp
 *
 *  Created on: Feb 9, 2011
 *      Author: wladt
 *  Removed to CWT on: Feb 12, 2013
 */

#include "pfs/dispatcher.hpp"
#include "pfs/safeformat.hpp"
#include "pfs/logger.hpp"
#include "pfs/fs.hpp"
#include "pfs/dl.hpp"

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

dispatcher::dispatcher (dispatcher::mapping_type mapping[], int n)
	: _masterModule(nullptr)
{
	for (int i = 0; i < n; i++) {
		_mapping.insert(mapping[i].id, & mapping[i]);
	}
}

dispatcher::~dispatcher ()
{
    disconnectAll();
    unregisterAll();
    pfs::log::print(_nx);
    _nx.clear();
}

void dispatcher::addSearchPath (const string & dir)
{
	pfs::dl & dl = pfs::dl::getDL();
	dl.addSearchPath(dir);
}

module * dispatcher::registerLocalModule (module * mod, module_dtor_t dtor)
{
	if (mod)
		return registerModule(*mod, nullptr, dtor) ? mod : nullptr;
	return nullptr;
}

module * dispatcher::registerModuleForPath (const string & path, const char * pname, int arg, const char ** argv)
{
	fs fs;

	if (!fs.exists(path)) {
		__notify_error(_nx, path, _u8("module not found by specified path or may be inconsistent"));
		return nullptr;
	}

	pfs::dl & dl = pfs::dl::getDL();
	dl::handle ph = dl.open(path, false, true);

	if (ph) {
		module_ctor_t module_ctor = reinterpret_cast<module_ctor_t>(dl.ptr(ph, PFS_MODULE_CTOR_NAME));
		module_dtor_t module_dtor = reinterpret_cast<module_dtor_t>(dl.ptr(ph, PFS_MODULE_DTOR_NAME));

		if (module_ctor) {
			module * mod = reinterpret_cast<module*>(module_ctor(pname, arg, argv));
			if (mod) {
				return registerModule(*mod, ph, module_dtor) ? mod : nullptr;
			} else {
				__notify_error(_nx, path, _u8("failed to construct module specified by path"));
			}
		} else {
			__notify_error(_nx, path, _u8("constructor not found for module specified by path"));
		}
		dl.close(ph);
	} else {
		__notify_error(_nx, path, _u8("failed to open module specified by path"));
	}
	return nullptr;
}

module * dispatcher::registerModuleForName (const string & name, const char * modname, int arg, const char ** argv)
{
	pfs::dl & dl = pfs::dl::getDL();
	string filename = dl.buildDlFileName(name);
	string realPath;
	bool global = false;  // Avoid name conflicts
	bool resolve = true;
	dl::handle ph = dl.open(filename, & realPath, global, resolve); // try to find module

	if (ph) {
		dl.close(ph);
		string msg;
		msg.append(_u8("module found at "));
		msg.append(realPath);
		__notify_debug(_nx, name, msg);
		return registerModuleForPath(realPath, modname, arg, argv);
	} else {
		// FIXME need dl to use notification instead of errorable_ext
		// and replace below code with
		// _nx.append(dl.get_notification())
		_nx.append(dl.lastErrorText());
	}

	__notify_error(_nx, name, _u8("module not found by specified name or may be inconsistent"));
	return nullptr;
}

bool dispatcher::registerModule (module & mod, dl::handle ph, module_dtor_t dtor)
{
	int nemitters, ndetectors;

	if (_modules.find(mod._name) != _modules.cend()) {
		__notify_error(_nx, mod._name, _u8("module already registered"));
		return false;
	}

	const emitter_mapping * emitters = mod.getEmitters(& nemitters);
	const detector_mapping * detectors = mod.getDetectors(& ndetectors);

	mapping_collection_type::iterator itEnd = _mapping.end();

	if (emitters) {
		for (int i = 0; i < nemitters; i++) {
			int emitter_id(emitters[i]._id);
			mapping_collection_type::iterator it = _mapping.find(emitter_id);
			if (it != itEnd) {
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
			mapping_collection_type::iterator it = _mapping.find(detector_id);
			if (it != itEnd) {
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

	mod.moduleRegistered.connect(this, & dispatcher::onModuleRegistered);
	mod._dispatcherPtr = this;
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

void dispatcher::connectAll ()
{
	mapping_collection_type::iterator it = _mapping.begin();
	mapping_collection_type::iterator itEnd = _mapping.end();

	for(; it != itEnd; it++ ) {
		it.value()->map->connectAll();
	}
}

void dispatcher::disconnectAll ()
{
	mapping_collection_type::iterator it = _mapping.begin();
	mapping_collection_type::iterator itEnd = _mapping.end();

	for(; it != itEnd; it++ ) {
		it.value()->map->disconnectAll();
	}
}

void dispatcher::unregisterAll ()
{
	vector<thread *>::iterator itThread = _threads.begin();
	vector<thread *>::iterator itThreadEnd = _threads.end();

	for (; itThread != itThreadEnd; ++itThread) {
		delete *itThread;
	}
	_threads.clear();

	module_specs_type::iterator it = _modules.begin();
	module_specs_type::iterator itEnd = _modules.end();

	for (;it != itEnd; it++) {
		module_spec pspec = it.value();
		module * mod = pspec.mod;
		PFS_ASSERT(mod);
		mod->moduleRegistered.disconnect(this);
		string pname = mod->name();

		if (pspec.dtor) {
			pspec.dtor(mod);
		}

		if (pspec.ph) {
			pfs::dl & dl = pfs::dl::getDL();
			dl.close(pspec.ph);
		}

		__notify_debug(_nx, pname, _u8("module unregistered"));
	}
	_modules.clear();
}

bool dispatcher::start ()
{
	bool r = true;

	module_specs_type::iterator it = _modules.begin();
	module_specs_type::iterator itEnd = _modules.end();

	for (;it != itEnd; it++) {
		module_spec pspec = it.value();
		module * mod = pspec.mod;
		PFS_ASSERT(mod);

		if (!mod->onStart(_nx)) {
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
	if (_masterModule) {
		size_t size = _threads.size();
		for (size_t i = 0; i < size; ++i) {
			const thread * th = _threads[i];
			const module_threaded * pt = dynamic_cast<const module_threaded*>(th);
			if (pt->modulePtr() == _masterModule) {
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

	if (_masterModule && _masterModule->run) {
		r = _masterModule->run(_masterModule);
	}

	for (itThread = _threads.begin(); itThread != itThreadEnd; ++itThread) {
		thread * t = *itThread;
		t->wait();
	}

	module_specs_type::iterator itModule = _modules.begin();
	module_specs_type::iterator itModuleEnd = _modules.end();

	for (; itModule != itModuleEnd; itModule++) {
		module_spec modspec = itModule.value();
		module * mod = modspec.mod;
		PFS_ASSERT(mod);
		mod->onFinish();
	}

	return r;
}

} // pfs

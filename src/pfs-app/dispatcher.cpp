/*
 * dispacther.cpp
 *
 *  Created on: Feb 9, 2011
 *      Author: wladt
 *  Removed to CWT on: Feb 12, 2013
 */
#include <pfs/logger.hpp>
#include <pfs/safeformat.hpp>
#include "pfs/dispatcher.hpp"

namespace pfs {

struct module_deleter
{
	module_dtor_t _deleter;

	module_deleter (module_dtor_t deleter)
		: _deleter(deleter)
	{}

	void operator () (module * p) const {
		_deleter(p);
	}
};


class module_threaded : public thread
{
public:
	module_threaded (shared_ptr<module> pmodule)
		: thread()
		, _pmodule(pmodule)
	{
		PFS_ASSERT(_pmodule);
	}

	virtual ~module_threaded ()
	{}

	const module * module_ptr () const
	{
		return _pmodule.get();
	}

protected:
	virtual void run ()
	{
		_pmodule->run(_pmodule.get());
	}

private:
	shared_ptr<module> _pmodule;
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

dispatcher::dispatcher (api_item_type * mapping, int n)
	: _master_module(0)
{
	for (int i = 0; i < n; i++) {
		_api.insert(api_type::value_type(mapping[i].id, & mapping[i]));
	}
}

void dispatcher::finalize ()
{
	if (_module_spec_map.size() > 0) {
		disconnect_all();
		unregister_all();

		if (_nx.count() > 0) {
			pfs::log::print(_nx);
			_nx.clear();
		}
	}
}

module_spec dispatcher::module_for_path (const fs::path & path
			, const char * class_name
			, void * mod_data)
{
	dynamic_library dl;
	error_code ex;
	string extended_errstr;

	fs::path dlpath(path);

	if (path.is_relative()) {
		dlpath = fs::join(fs::path("."), path);
	}

	if (not dl.open(dlpath, _searchdirs, & ex, & extended_errstr)) {
		__notify_error(_nx, dlpath.native(), to_string(ex));

		if (not extended_errstr.empty())
			__notify_error(_nx, dlpath.native(), extended_errstr);

		return module_spec();
	}

	dynamic_library::symbol ctor = dl.resolve(PFS_MODULE_CTOR_NAME
			, & ex
			, & extended_errstr);

	if (!ctor) {
		__notify_error(_nx, dlpath.native(), _u8("Failed to resolve `ctor' for module"));
		__notify_error(_nx, dlpath.native(), to_string(ex));

		if (not extended_errstr.empty())
			__notify_error(_nx, dlpath.native(), extended_errstr);

		return module_spec();
	}

	dynamic_library::symbol dtor = dl.resolve(PFS_MODULE_DTOR_NAME
			, & ex
			, & extended_errstr);

	if (!dtor) {
		__notify_error(_nx, dlpath.native(), _u8("Failed to resolve `dtor' for module"));
		__notify_error(_nx, dlpath.native(), to_string(ex));

		if (not extended_errstr.empty())
			__notify_error(_nx, dlpath.native(), extended_errstr);

		return module_spec();
	}

	module_ctor_t module_ctor = reinterpret_cast<module_ctor_t>(ctor);
	module_dtor_t module_dtor = reinterpret_cast<module_dtor_t>(dtor);

	module * ptr = reinterpret_cast<module *>(module_ctor(class_name, mod_data));

	if (!ptr)
		return module_spec();

	module_spec result;
	result.dl = dl;
	result.pmodule = shared_ptr<module>(ptr, module_deleter(module_dtor));

	return result;
}

bool dispatcher::register_module (const module_spec & modspec)
{
	int nemitters, ndetectors;

	if (modspec.pmodule.is_null())
		return false;

	shared_ptr<module> pmodule = modspec.pmodule;

	if (_module_spec_map.find(pmodule->name()) != _module_spec_map.end()) {
		__notify_error(_nx, pmodule->name(), _u8("Module already registered"));
		return false;
	}

	const emitter_mapping * emitters = pmodule->get_emitters(& nemitters);
	const detector_mapping * detectors = pmodule->get_detectors(& ndetectors);

	api_type::iterator it_end = _api.end();

	if (emitters) {
		for (int i = 0; i < nemitters; i++) {
			int emitter_id(emitters[i]._id);
			api_type::iterator it = _api.find(emitter_id);

			if (it != it_end) {
				it->second->map->append_emitter(reinterpret_cast<emitter *>(emitters[i]._emitter));
			} else {
				//safeformat sf(_u8("Emitter '%s' not found while registering module, may be signal/slot mapping is not supported for this application"))(to_string(emitters[i]._id));
				safeformat sf(_u8("Emitter '%s' not found while registering module, "
						"may be signal/slot mapping is not supported for this application"));
						sf.arg(to_string(emitters[i]._id));
				__notify_warn(_nx, pmodule->name(), sf.str());
			}
		}
	}

	if (detectors) {
		for (int i = 0; i < ndetectors; i++) {
			int detector_id(detectors[i]._id);
			api_type::iterator it = _api.find(detector_id);

			if (it != it_end) {
				it->second->map->append_detector(pmodule.get(), detectors[i]._detector);
			} else {
				safeformat sf(_u8("Detector '%s' not found while registering module, "
						"may be signal/slot mapping is not supported for this application"));
				sf.arg(to_string(detectors[i]._id));
				__notify_warn(_nx, pmodule->name(), sf.str());
			}
		}
	}

	pmodule->emit_module_registered.connect(this, & dispatcher::on_module_registered);
	pmodule->set_dispatcher(this);

	_module_spec_map.insert(module_spec_map_type::value_type(pmodule->name(), modspec));

	// Module must be run in a separate thread.
	//
	if (pmodule->run) {
		_threads.push_back(new module_threaded(pmodule));
		__notify_debug(_nx, pmodule->name(), _u8("Module registered as threaded"));
	} else {
		__notify_debug(_nx, pmodule->name(), _u8("Module registered"));
	}

	return true;
}

void dispatcher::set_master_module (const string & name)
{
	module_spec_map_type::iterator it = _module_spec_map.begin();
	module_spec_map_type::iterator itEnd = _module_spec_map.end();

	for (;it != itEnd; it++) {
		module_spec modspec = it->second;
		shared_ptr<module> pmodule = modspec.pmodule;

		if (pmodule->name() == name)
			_master_module = pmodule;
	}
}

void dispatcher::connect_all ()
{
	api_type::iterator it = _api.begin();
	api_type::iterator itEnd = _api.end();

	for(; it != itEnd; it++ ) {
		it->second->map->connect_all();
	}
}

void dispatcher::disconnect_all ()
{
	api_type::iterator it = _api.begin();
	api_type::iterator itEnd = _api.end();

	for(; it != itEnd; it++ ) {
		it->second->map->disconnect_all();
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

	module_spec_map_type::iterator it = _module_spec_map.begin();
	module_spec_map_type::iterator itEnd = _module_spec_map.end();

	for (;it != itEnd; it++) {
		module_spec modspec = it->second;
		shared_ptr<module> pmodule = modspec.pmodule;
		pmodule->emit_module_registered.disconnect(this);

//		if (pspec.dtor) {
//			pspec.dtor(mod);
//		}
//		if (pspec.dl) {
//			pfs::dynamic_library & dl = pfs::dynamic_library::getDL();
//			dl.close(pspec.dl);
//		}

		__notify_debug(_nx, pmodule->name(), _u8("Module unregistered"));
	}

	_module_spec_map.clear();
}

bool dispatcher::start ()
{
	bool r = true;

	module_spec_map_type::iterator it = _module_spec_map.begin();
	module_spec_map_type::iterator itEnd = _module_spec_map.end();

	for (;it != itEnd; it++) {
		module_spec modspec = it->second;
		shared_ptr<module> pmodule = modspec.pmodule;

		if (!pmodule->on_start(_nx)) {
			__notify_error(_nx, pmodule->name(), _u8("Failed to start module"));
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
	//
	if (_master_module) {
		size_t size = _threads.size();

		for (size_t i = 0; i < size; ++i) {
			const thread * th = _threads[i];
			const module_threaded * pt = dynamic_cast<const module_threaded*>(th);

			if (pt->module_ptr() == _master_module.get()) {
				_threads.erase(_threads.begin() + i);
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
		r = _master_module->run(_master_module.get());
	}

	for (itThread = _threads.begin(); itThread != itThreadEnd; ++itThread) {
		thread * t = *itThread;
		t->wait();
	}

	module_spec_map_type::iterator itModule = _module_spec_map.begin();
	module_spec_map_type::iterator itModuleEnd = _module_spec_map.end();

	for (; itModule != itModuleEnd; itModule++) {
		module_spec modspec = itModule->second;
		modspec.pmodule->on_finish();
	}

	return r;
}

} // pfs

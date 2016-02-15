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

dispatcher::dispatcher (api_item_type * mapping, int n)
	: _master_module(0)
{
	for (int i = 0; i < n; ++i) {
		_api.insert(api_type::value_type(mapping[i].id, & mapping[i]));
	}
}

inline pfs::string __build_string_for_log (const module * m, const string & s)
{
	if (!m)
		return s;

	pfs::string r(m->name());
	r.append(": ");
	r.append(s);

	return r;
}

void dispatcher::print_info (const module * m, const string & s)
{
	pfs::info(__build_string_for_log(m, s));
	emit_info(m, s);
}

void dispatcher::print_debug (const module * m, const string & s)
{
	pfs::debug(__build_string_for_log(m, s));
	emit_debug(m, s);
}

void dispatcher::print_warn  (const module * m, const string & s)
{
	pfs::warn(__build_string_for_log(m, s));
	emit_warn(m, s);
}

void dispatcher::print_error (const module * m, const string & s)
{
	pfs::error(__build_string_for_log(m, s));
	emit_error(m, s);
}

void dispatcher::finalize ()
{
	if (_module_spec_map.size() > 0) {
		disconnect_all();
		unregister_all();
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
		print_error(0, _Sf("%s: %s")(dlpath.native())(to_string(ex)).str());

		if (not extended_errstr.empty()) {
			print_error(0, _Sf("%s: %s")(dlpath.native())(extended_errstr).str());
		}

		return module_spec();
	}

	dynamic_library::symbol ctor = dl.resolve(PFS_MODULE_CTOR_NAME
			, & ex
			, & extended_errstr);

	if (!ctor) {
		print_error(0, _Sf("%s: Failed to resolve `ctor' for module")(dlpath.native()).str());
		print_error(0, _Sf("%s: %s")(dlpath.native())(to_string(ex)).str());

		if (not extended_errstr.empty()) {
			print_error(0, _Sf("%s: %s")(dlpath.native())(extended_errstr).str());
		}

		return module_spec();
	}

	dynamic_library::symbol dtor = dl.resolve(PFS_MODULE_DTOR_NAME
			, & ex
			, & extended_errstr);

	if (!dtor) {
		print_error(0, _Sf("%s: Failed to resolve `dtor' for module")(dlpath.native()).str());
		print_error(0, _Sf("%s: %s")(dlpath.native())(to_string(ex)).str());

		if (not extended_errstr.empty()) {
			print_error(0, _Sf("%s: %s")(dlpath.native())(extended_errstr).str());
		}

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
		print_error(0, _Sf("%s: Module already registered")(pmodule->name()).str());
		return false;
	}

	const emitter_mapping * emitters = pmodule->get_emitters(& nemitters);
	const detector_mapping * detectors = pmodule->get_detectors(& ndetectors);

	api_type::iterator it_end = _api.end();

	if (emitters) {
		for (int i = 0; i < nemitters; ++i) {
			int emitter_id(emitters[i]._id);
			api_type::iterator it = _api.find(emitter_id);

			if (it != it_end) {
				it->second->map->append_emitter(reinterpret_cast<emitter *>(emitters[i]._emitter));
			} else {
				print_warn(0, _Sf("%s: Emitter '%s' not found while registering module, "
						"may be signal/slot mapping is not supported for this application")
						(pmodule->name())
						(to_string(emitters[i]._id)).str());
			}
		}
	}

	if (detectors) {
		for (int i = 0; i < ndetectors; ++i) {
			int detector_id(detectors[i]._id);
			api_type::iterator it = _api.find(detector_id);

			if (it != it_end) {
				it->second->map->append_detector(pmodule.get(), detectors[i]._detector);
			} else {
				print_warn(0, _Sf(_u8("%s: Detector '%s' not found while registering module, "
						"may be signal/slot mapping is not supported for this application"))
						(pmodule->name())
						(to_string(detectors[i]._id)).str());
			}
		}
	}

	pmodule->emit_module_registered.connect(this, & dispatcher::module_registered);
	pmodule->set_dispatcher(this);

	_module_spec_map.insert(module_spec_map_type::value_type(pmodule->name(), modspec));

	// Module must be run in a separate thread.
	//
	if (pmodule->run) {
		_threads.push_back(new module_threaded(pmodule));
		print_debug(0, _Sf("%s: Module registered as threaded")(pmodule->name()).str());
	} else {
		print_debug(0, _Sf("%s: Module registered")(pmodule->name()).str());
	}

	return true;
}

void dispatcher::set_master_module (const string & name)
{
	module_spec_map_type::iterator it = _module_spec_map.begin();
	module_spec_map_type::iterator itEnd = _module_spec_map.end();

	for (;it != itEnd; ++it) {
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

	for (; it != itEnd; ++it) {
		it->second->map->connect_all();
	}
}

void dispatcher::disconnect_all ()
{
	api_type::iterator it = _api.begin();
	api_type::iterator itEnd = _api.end();

	for (; it != itEnd; ++it) {
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

	for (;it != itEnd; ++it) {
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

		print_debug(0, _Sf("%s: Module unregistered")(pmodule->name()).str());
	}

	_module_spec_map.clear();
}

bool dispatcher::start ()
{
	bool r = true;

	module_spec_map_type::iterator it = _module_spec_map.begin();
	module_spec_map_type::iterator itEnd = _module_spec_map.end();

	for (;it != itEnd; ++it) {
		module_spec modspec = it->second;
		shared_ptr<module> pmodule = modspec.pmodule;

		pmodule->emit_quit.connect (this, & dispatcher::broadcast_quit);
		this->emit_quit.connect(pmodule.get(), & module::on_quit);

		pmodule->emit_info.connect (this, & dispatcher::print_info);
		pmodule->emit_debug.connect(this, & dispatcher::print_debug);
		pmodule->emit_warn.connect (this, & dispatcher::print_warn);
		pmodule->emit_error.connect(this, & dispatcher::print_error);

		if (not pmodule->on_start()) {
			print_error(pmodule.get(), _u8("Failed to start module"));
			r = false;
		}
	}

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

	for (; itModule != itModuleEnd; ++itModule) {
		module_spec modspec = itModule->second;
		shared_ptr<module> pmodule = modspec.pmodule;
		pmodule->on_finish();

		pmodule->emit_info.disconnect(this);
		pmodule->emit_debug.disconnect(this);
		pmodule->emit_warn.disconnect(this);
		pmodule->emit_error.disconnect(this);
	}

	return r;
}

void module::connect_info (log_consumer * p)
{
	_pdispatcher->emit_info.connect(p, & log_consumer::_on_info);
}

void module::connect_debug (log_consumer * p)
{
	_pdispatcher->emit_debug.connect(p, & log_consumer::_on_debug);
}

void module::connect_warn (log_consumer * p)
{
	_pdispatcher->emit_warn.connect(p, & log_consumer::_on_warn);
}

void module::connect_error (log_consumer * p)
{
	_pdispatcher->emit_error.connect(p, & log_consumer::_on_error);
}


} // pfs

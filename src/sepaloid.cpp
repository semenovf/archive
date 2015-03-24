/*
 * sepaloid.cpp
 *
 *  Created on: Feb 9, 2011
 *      Author: wladt
 *  Removed to CWT on: Feb 12, 2013
 */

#include "pfs/sepaloid.hpp"
#include "pfs/safeformat.hpp"
#include "pfs/logger.hpp"
#include "pfs/fs.hpp"
#include "pfs/dl.hpp"

namespace pfs {

class petaloid_threaded : public thread
{
public:
	petaloid_threaded (petaloid * p) : thread() , _petaloid(p) { PFS_ASSERT(_petaloid); }
	virtual ~petaloid_threaded () {}
	const petaloid * petaloidPtr () const { return _petaloid; }

protected:
	virtual void run() { _petaloid->run(_petaloid); }

private:
	petaloid * _petaloid;
};

sepaloid::sepaloid (sepaloid::mapping_type mapping[], int n)
	: _masterPetaloid(nullptr)
{
	for (int i = 0; i < n; i++) {
		_mapping.insert(mapping[i].id, & mapping[i]);
	}
}

petaloid * sepaloid::registerLocalPetaloid (petaloid * petaloid, petaloid_dtor_t dtor)
{
	if (petaloid)
		return registerPetaloid(*petaloid, nullptr, dtor) ? petaloid : nullptr;
	return nullptr;
}

petaloid * sepaloid::registerPetaloidForPath (const string & path, const char * pname, int arg, char ** argv)
{
	fs fs;

	if (!fs.exists(path)) {
		string m;
		this->addError(m << _Tr("petaloid not found by specified path or may be inconsistent" << ": " << path));
		return nullptr;
	}

	dl::handle ph = dl::open(path, false, true);
	if (ph) {
		petaloid_ctor_t petaloid_ctor = reinterpret_cast<petaloid_ctor_t>(dl::ptr(ph, CWT_PETALOID_CONSTRUCTOR_NAME));
		petaloid_dtor_t petaloid_dtor = reinterpret_cast<petaloid_dtor_t>(dl::ptr(ph, CWT_PETALOID_DESTRUCTOR_NAME));

		if (petaloid_ctor) {
			petaloid * p = reinterpret_cast<petaloid*>(petaloid_ctor(pname, arg, argv));
			if (p) {
				return registerPetaloid(*p, ph, petaloid_dtor) ? p : nullptr;
			} else {
				string m;
				this->addError(m << _Tr("failed to construct petaloid specified by path") << ": " << path);
			}
		} else {
			string m;
			this->addError(m << _Tr("constructor not found for petaloid specified by path") << ": " << path);
		}
		dl::close(ph);
	} else {
		string m;
		this->addError(m << _Tr("failed to open petaloid specified by path") << ": " << path);
	}
	return nullptr;
}

petaloid * sepaloid::registerPetaloidForName (const string & name, const char * pname, int arg, char ** argv)
{
	string filename = dl::buildDlFileName(name);
	string realPath;
	bool global = false;  // Avoid name conflicts
	bool resolve = true;
	dl::handle ph = dl::open(filename, realPath, global, resolve); // try to find petaloid

	if (ph) {
		dl::close(ph);
		string m(name);
		debug(m << ": " << _Tr("petaloid found at ") << realPath);
		return registerPetaloidForPath(realPath, pname, arg, argv);
	}

	string m(name);
	this->addError(m << ": " << _Tr("petaloid not found by specified name or may be inconsistent"));
	return nullptr;
}

bool sepaloid::registerPetaloid (petaloid & petaloid, dl::handle ph, petaloid_dtor_t dtor)
{
	int nemitters, ndetectors;

	if (_petaloids.find(petaloid._name) != _petaloids.cend()) {
		string m(petaloid._name);
		this->addError(m << ": " << _Tr("petaloid already registered"));
		return false;
	}

	const emitter_mapping * emitters = petaloid.getEmitters(& nemitters);
	const detector_mapping * detectors = petaloid.getDetectors(& ndetectors);

	mapping_hash::iterator itEnd = _mapping.end();

	if (emitters) {
		for (int i = 0; i < nemitters; i++) {
			int emitter_id(emitters[i]._id);
			mapping_hash::iterator it = _mapping.find(emitter_id);
			if (it != itEnd) {
				it.value()->map->appendEmitter(reinterpret_cast<emitter *>(emitters[i]._emitter));
			} else {
				string m(petaloid.name());
				warn(m << ": " << _Tr("emitter") << " '" << emitters[i]._id << "' " _Tr("not found while registering petaloid ..."));
				warn(_Tr("... may be signal/slot mapping is not supported for this application"));
			}
		}
	}

	if (detectors) {
		for (int i = 0; i < ndetectors; i++) {
			int detector_id(detectors[i]._id);
			mapping_hash::iterator it = _mapping.find(detector_id);
			if (it != itEnd) {
				it.value()->map->appendDetector(& petaloid, detectors[i]._detector);
			} else {
				string m(petaloid.name());
				warn(m << ": " << _Tr("detector") << " '" << emitters[i]._id << "' "
					<< _Tr("not found while registering petaloid ..."));
				warn(_Tr("... may be signal/slot mapping is not supported for this application"));
			}
		}
	}

	petaloid.petaloidRegistered.connect(this, & sepaloid::onPetaloidRegistered);
	petaloid._sepaloidPtr = this;
	petaloid_spec pspec(& petaloid, ph, dtor);
	_petaloids.insert(petaloid._name, pspec);

	// petaloid must be run in a separate thread.
	if (petaloid.run) {
		_threads.append(new petaloid_threaded(& petaloid));
		string m(petaloid.name());
		debug(m << ": " << _Tr("petaloid registered as threaded"));
	} else {
		string m(petaloid.name());
		debug(m << ": " << _Tr("petaloid registered"));
	}

	return true;
}

void sepaloid::connectAll ()
{
	mapping_hash::iterator it = _mapping.begin();
	mapping_hash::iterator itEnd = _mapping.end();

	for(; it != itEnd; it++ ) {
		it.value()->map->connectAll();
	}
}

void sepaloid::disconnectAll ()
{
	mapping_hash::iterator it = _mapping.begin();
	mapping_hash::iterator itEnd = _mapping.end();

	for(; it != itEnd; it++ ) {
		it.value()->map->disconnectAll();
	}
}

void sepaloid::unregisterAll ()
{
	vector<thread *>::iterator itThread = _threads.begin();
	vector<thread *>::iterator itThreadEnd = _threads.end();

	for (; itThread != itThreadEnd; ++itThread) {
		delete *itThread;
	}
	_threads.clear();

	petaloid_specs_type::iterator it = _petaloids.begin();
	petaloid_specs_type::iterator itEnd = _petaloids.end();

	for (;it != itEnd; it++) {
		petaloid_spec pspec = it.value();
		petaloid * p = pspec.p;
		PFS_ASSERT(p);
		p->petaloidRegistered.disconnect(this);
		string pname = p->name();
		if (pspec.dtor) {
			pspec.dtor(p);
		}
		if (pspec.ph) {
			dl::close(pspec.ph);
		}
		string m(pname);
		debug(m << ": " << _Tr("petaloid unregistered"));
	}
	_petaloids.clear();
}

bool sepaloid::start ()
{
	bool r = true;

	petaloid_specs_type::iterator it = _petaloids.begin();
	petaloid_specs_type::iterator itEnd = _petaloids.end();

	for (;it != itEnd; it++) {
		petaloid_spec pspec = it.value();
		petaloid * p = pspec.p;
		PFS_ASSERT(p);

		if (!p->onStart()) {
			string m(p->name());
			this->addError(m << ": " << _Tr("failed to start petaloid"));
			r = false;
		}
	}

	return r;
}

int sepaloid::exec ()
{
	int r = 0;

	// Exclude master petaloid from sepaloid's threads pool
	if (_masterPetaloid) {
		size_t size = _threads.size();
		for (size_t i = 0; i < size; ++i) {
			const thread * th = _threads[i];
			const petaloid_threaded * pt = dynamic_cast<const petaloid_threaded*>(th);
			if (pt->petaloidPtr() == _masterPetaloid) {
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

	if (_masterPetaloid && _masterPetaloid->run) {
		r = _masterPetaloid->run(_masterPetaloid);
	}

	for (itThread = _threads.begin(); itThread != itThreadEnd; ++itThread) {
		thread * t = *itThread;
		t->wait();
	}

	petaloid_specs_type::iterator itPetaloid = _petaloids.begin();
	petaloid_specs_type::iterator itPetaloidEnd = _petaloids.end();

	for (; itPetaloid != itPetaloidEnd; itPetaloid++) {
		petaloid_spec pspec = itPetaloid.value();
		petaloid * p = pspec.p;
		PFS_ASSERT(p);
		p->onFinish();
	}

	return r;
}

} // pfs

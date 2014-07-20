/*
 * sepaloid.cpp
 *
 *  Created on: Feb 9, 2011
 *      Author: wladt
 *  Removed to CWT on: Feb 12, 2013
 */

#include "../include/cwt/sepaloid.hpp"
#include <cwt/safeformat.hpp>
#include <cwt/logger.hpp>
#include <cwt/fs.hpp>
#include <cwt/dl.hpp>

namespace cwt {

class petaloid_threaded : public cwt::thread
{
public:
	petaloid_threaded (cwt::petaloid * p) : cwt::thread() , m_petaloid(p) { PFS_ASSERT(m_petaloid); }
	virtual ~petaloid_threaded () {}
	const cwt::petaloid * petaloid () const { return m_petaloid; }

protected:
	virtual void run() { m_petaloid->run(m_petaloid); }

private:
	cwt::petaloid * m_petaloid;
};

sepaloid::sepaloid (sepaloid::mapping_type mapping[], int n)
	: _masterPetaloid(nullptr)
{
	for (int i = 0; i < n; i++) {
		_mapping.insert(mapping[i].id, & mapping[i]);
	}
}

cwt::petaloid * sepaloid::registerLocalPetaloid (cwt::petaloid * petaloid, petaloid_dtor_t dtor)
{
	if (petaloid)
		return registerPetaloid(*petaloid, nullptr, dtor) ? petaloid : nullptr;
	return nullptr;
}

cwt::petaloid * sepaloid::registerPetaloidForPath (const pfs::string & path, const char * pname, int arg, char ** argv)
{
	cwt::fs fs;

	if (!fs.exists(path)) {
		this->addError(_Fr("petaloid not found by specified path or may be inconsistent: %s") % path);
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
				this->addError(_Fr("failed to construct petaloid specified by path: %s") % path);
			}
		} else {
			this->addError(_Fr("constructor not found for petaloid specified by path: %s") % path);
		}
		cwt::dl::close(ph);
	} else {
		this->addError(_Fr("failed to open petaloid specified by path: %s") % path);
	}
	return nullptr;
}

petaloid * sepaloid::registerPetaloidForName (const pfs::string & name, const char * pname, int arg, char ** argv)
{
	pfs::string filename = dl::buildDlFileName(name);
	pfs::string realPath;
	bool global = false;  // Avoid name conflicts
	bool resolve = true;
	dl::handle ph = dl::open(filename, realPath, global, resolve); // try to find petaloid

	if (ph) {
		dl::close(ph);
		cwt::debug(_Fr("%s: petaloid found at '%s'") % name % realPath);
		return registerPetaloidForPath(realPath, pname, arg, argv);
	}

	this->addError(_Fr("%s: petaloid not found by specified name or may be inconsistent") % name);
	return nullptr;
}

bool sepaloid::registerPetaloid (petaloid & petaloid, dl::handle ph, petaloid_dtor_t dtor)
{
	int nemitters, ndetectors;

	if (_petaloids.find(petaloid._name) != _petaloids.cend()) {
		this->addError(_Fr("%s: petaloid already registered") % petaloid._name);
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
				it->second->map->appendEmitter(reinterpret_cast<emitter *>(emitters[i]._emitter));
			} else {
				cwt::warn(
					_Fr("%s: emitter '%s' not found while registering petaloid ...")
					% petaloid.name()
					% emitters[i]._id);
				cwt::warn(_Tr("... may be signal/slot mapping is not supported for this application"));
			}
		}
	}

	if (detectors) {
		for (int i = 0; i < ndetectors; i++) {
			int detector_id(detectors[i]._id);
			mapping_hash::iterator it = _mapping.find(detector_id);
			if (it != itEnd) {
				it->second->map->appendDetector(& petaloid, detectors[i]._detector);
			} else {
				cwt::warn(
					_Fr("%s: detector '%s' not found while registering petaloid ...")
					% petaloid.name()
					% emitters[i]._id);
				cwt::warn(_Tr("... may be signal/slot mapping is not supported for this application"));
			}
		}
	}

	petaloid.petaloidRegistered.connect(this, & sepaloid::onPetaloidRegistered);
	petaloid._sepaloidPtr = this;
	petaloid_spec pspec(& petaloid, ph, dtor);
	_petaloids.insert(petaloid._name, pspec);


	// petaloid must be run in a separate thread.
	if (petaloid.run) {
		_threads.append(new petaloid_threaded(&petaloid));
		cwt::debug(_Fr("%s: petaloid registered as threaded") % petaloid.name());
	} else {
		cwt::debug(_Fr("%s: petaloid registered") % petaloid.name());
	}

	return true;
}

void sepaloid::connectAll ()
{
	mapping_hash::iterator it = _mapping.begin();
	mapping_hash::iterator itEnd = _mapping.end();

	for(; it != itEnd; it++ ) {
		it->second->map->connectAll();
	}
}

void sepaloid::disconnectAll ()
{
	mapping_hash::iterator it = _mapping.begin();
	mapping_hash::iterator itEnd = _mapping.end();

	for(; it != itEnd; it++ ) {
		it->second->map->disconnectAll();
	}
}

void sepaloid::unregisterAll ()
{
	pfs::vector<cwt::thread *>::iterator itThread = _threads.begin();
	pfs::vector<cwt::thread *>::iterator itThreadEnd = _threads.end();

	for (; itThread != itThreadEnd; ++itThread) {
		delete *itThread;
	}
	_threads.clear();

	petaloid_specs_type::iterator it = _petaloids.begin();
	petaloid_specs_type::iterator itEnd = _petaloids.end();

	for (;it != itEnd; it++) {
		PFS_ASSERT(it->second.p);
		it->second.p->petaloidRegistered.disconnect(this);
		pfs::string pname = it->second.p->name();
		if (it->second.dtor) {
			it->second.dtor(it->second.p);
			it->second.p = nullptr;
		}
		if (it->second.ph) {
			dl::close(it->second.ph);
		}
		cwt::debug(_Fr("%s: petaloid unregistered") % pname);
	}
	_petaloids.clear();
}

bool sepaloid::start ()
{
	bool r = true;

	petaloid_specs_type::iterator it = _petaloids.begin();
	petaloid_specs_type::iterator itEnd = _petaloids.end();

	for (;it != itEnd; it++) {
		PFS_ASSERT(it->second.p);
		if (!it->second.p->onStart()) {
			this->addError(_Fr("%s: failed to start petaloid") % it->second.p->name());
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
			const cwt::thread * th = _threads[i];
			const petaloid_threaded * pt = dynamic_cast<const petaloid_threaded*>(th);
			if (pt->petaloid() == _masterPetaloid) {
				_threads.remove(i);
				delete pt;
				break;
			}
		}
	}

	pfs::vector<cwt::thread *>::iterator itThread = _threads.begin();
	pfs::vector<cwt::thread *>::iterator itThreadEnd = _threads.end();

	for (; itThread != itThreadEnd; itThread++) {
		(*itThread)->start();
	}

	if (_masterPetaloid && _masterPetaloid->run) {
		r = _masterPetaloid->run(_masterPetaloid);
	}

	for (itThread = _threads.begin(); itThread != itThreadEnd; itThread++) {
		(*itThread)->wait();
	}

	petaloid_specs_type::iterator itPetaloid = _petaloids.begin();
	petaloid_specs_type::iterator itPetaloidEnd = _petaloids.end();

	for (; itPetaloid != itPetaloidEnd; itPetaloid++) {
		PFS_ASSERT(itPetaloid->second.p);
		itPetaloid->second.p->onFinish();
	}

	return r;
}

} // cwt

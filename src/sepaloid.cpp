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
	: m_masterPetaloid(nullptr)
{
	for (int i = 0; i < n; i++) {
		m_mapping.insert(mapping[i].id, & mapping[i]);
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
		log::error(_Fr("petaloid not found by specified path: %s") % path);
		return nullptr;
	}

	dl::handle ph = dl::open(path);
	if (ph) {
		petaloid_ctor_t petaloid_ctor = reinterpret_cast<petaloid_ctor_t>(dl::ptr(ph, CWT_PETALOID_CONSTRUCTOR_NAME));
		petaloid_dtor_t petaloid_dtor = reinterpret_cast<petaloid_dtor_t>(dl::ptr(ph, CWT_PETALOID_DESTRUCTOR_NAME));

		if (petaloid_ctor) {
			petaloid * p = reinterpret_cast<petaloid*>(petaloid_ctor(pname, arg, argv));
			if (p) {
				return registerPetaloid(*p, ph, petaloid_dtor) ? p : nullptr;
			} else {
				log::error(_Fr("Failed to construct petaloid specified by path: %s") % path);
			}
		} else {
			log::error(_Fr("Constructor not found for petaloid specified by path: %s") % path);
		}
		cwt::dl::close(ph);
	} else {
		log::error(_Fr("Failed to open petaloid specified by path: %s") % path);
	}
	return nullptr;
}

petaloid * sepaloid::registerPetaloidForName (const pfs::string & name, const char * pname, int arg, char ** argv)
{
	pfs::string filename = dl::buildDlFileName(name);
	pfs::string realPath;
	dl::handle ph = dl::open(filename, realPath); // try to find petaloid

	if (ph) {
		dl::close(ph);
		log::debug(_Fr("petaloid [%s] found at '%s'") % name % realPath);
		return registerPetaloidForPath(realPath, pname, arg, argv);
	}

	log::error(_Fr("petaloid not found by specified name: %s") % name);
	return nullptr;
}

bool sepaloid::registerPetaloid (petaloid & petaloid, dl::handle ph, petaloid_dtor_t dtor)
{
	int nemitters, ndetectors;
	const emitter_mapping * emitters = petaloid.getEmitters(& nemitters);
	const detector_mapping * detectors = petaloid.getDetectors(& ndetectors);

	mapping_hash::iterator itEnd = m_mapping.end();

	if (emitters) {
		for (int i = 0; i < nemitters; i++) {
			int emitter_id(emitters[i]._id);
			mapping_hash::iterator it = m_mapping.find(emitter_id);
			if (it != itEnd) {
				it->second->map->appendEmitter(reinterpret_cast<emitter *>(emitters[i]._emitter));
			} else {
				log::warn(
					_Fr("Emitter '%s' not found while registering petaloid [%s] ...")
					% emitters[i]._id
					% petaloid.name());
				log::warn(_Tr("... may be signal/slot mapping is not supported for this application"));
			}
		}
	}

	if (detectors) {
		for (int i = 0; i < ndetectors; i++) {
			int detector_id(detectors[i]._id);
			mapping_hash::iterator it = m_mapping.find(detector_id);
			if (it != itEnd) {
				it->second->map->appendDetector(& petaloid, detectors[i]._detector);
			} else {
				log::warn(
					_Fr("Detector '%s' not found while registering petaloid [%s] ...")
					% emitters[i]._id
					% petaloid.name());
				log::warn(_Tr("... may be signal/slot mapping is not supported for this application"));
			}
		}
	}

	petaloid.petaloidRegistered.connect(this, & sepaloid::onPetaloidRegistered);
	petaloid.m_sepaloidPtr = this;
	petaloid_spec pspec(&petaloid, ph, dtor);
	m_petaloids.append(pspec);



	// petaloid must be run in a separate thread.
	if (petaloid.run) {
		m_threads.append(new petaloid_threaded(&petaloid));
		log::debug(_Fr("petaloid [%s] registered as threaded") % petaloid.name());
	} else {
		log::debug(_Fr("petaloid [%s] registered") % petaloid.name());
	}

	return true;
}

void sepaloid::connectAll()
{
	mapping_hash::iterator it = m_mapping.begin();
	mapping_hash::iterator itEnd = m_mapping.end();

	for(; it != itEnd; it++ ) {
		it->second->map->connectAll();
	}
}

void sepaloid::disconnectAll()
{
	mapping_hash::iterator it = m_mapping.begin();
	mapping_hash::iterator itEnd = m_mapping.end();

	for(; it != itEnd; it++ ) {
		it->second->map->disconnectAll();
	}
}

void sepaloid::unregisterAll()
{
	pfs::vector<cwt::thread*>::iterator itThread = m_threads.begin();
	pfs::vector<cwt::thread*>::iterator itThreadEnd = m_threads.end();

	for (; itThread != itThreadEnd; ++itThread) {
		delete *itThread;
	}
	m_threads.clear();

	pfs::vector<petaloid_spec>::iterator it = m_petaloids.begin();
	pfs::vector<petaloid_spec>::iterator itEnd = m_petaloids.end();

	for (;it != itEnd; it++) {
		PFS_ASSERT(it->p);
		it->p->petaloidRegistered.disconnect(this);
		pfs::string pname = it->p->name();
		if (it->dtor) {
			it->dtor(it->p);
			it->p = nullptr;
		}
		if (it->ph) {
			dl::close(it->ph);
		}
		log::debug(_Fr("petaloid [%s] unregistered") % pname);
	}
	m_petaloids.clear();
}

void sepaloid::start()
{
	pfs::vector<petaloid_spec>::iterator it = m_petaloids.begin();
	pfs::vector<petaloid_spec>::iterator itEnd = m_petaloids.end();

	for (;it != itEnd; it++) {
		PFS_ASSERT(it->p);
		it->p->onStart();
	}
}

int sepaloid::exec()
{
	int r = 0;

	// Exclude master petaloid from threads pool of sepaloid
	if (m_masterPetaloid) {
		size_t size = m_threads.size();
		for (size_t i = 0; i < size; ++i) {
			petaloid_threaded * pt = dynamic_cast<petaloid_threaded*>(m_threads[i]);
			if (pt->petaloid() == m_masterPetaloid) {
				m_threads.remove(i);
				delete pt;
				break;
			}
		}
	}

	pfs::vector<cwt::thread*>::iterator itThread = m_threads.begin();
	pfs::vector<cwt::thread*>::iterator itThreadEnd = m_threads.end();

	for (;itThread != itThreadEnd; itThread++) {
		(*itThread)->start();
	}

	if (m_masterPetaloid && m_masterPetaloid->run) {
		r = m_masterPetaloid->run(m_masterPetaloid);
	}

	for (itThread = m_threads.begin(); itThread != itThreadEnd; itThread++) {
		(*itThread)->wait();
	}

	pfs::vector<petaloid_spec>::iterator itPetaloid = m_petaloids.begin();
	pfs::vector<petaloid_spec>::iterator itPetaloidEnd = m_petaloids.end();

	for (; itPetaloid != itPetaloidEnd; itPetaloid++) {
		PFS_ASSERT(itPetaloid->p);
		itPetaloid->p->onFinish();
	}

	return r;
}

/*
void sepaloid::quit()
{
	AutoLock(this);
	pfs::vector<cwt::thread*>::iterator itThread = m_threads.begin();
	pfs::vector<cwt::thread*>::iterator itThreadEnd = m_threads.end();

	for (;itThread != itThreadEnd; itThread++) {
		(*itThread)->quit();
	}
}
*/

} // cwt

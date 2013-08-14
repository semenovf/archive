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
#include <cwt/filesystem.hpp>
#include <cwt/dl.hpp>

CWT_NS_BEGIN

class PetaloidThreaded : public Thread
{
public:
	PetaloidThreaded(Petaloid *p) : Thread() , m_petaloid(p) { CWT_ASSERT(m_petaloid); }
	virtual ~PetaloidThreaded() {}
	const Petaloid *petaloid() const { return m_petaloid; }

protected:
	virtual void run() { m_petaloid->run(m_petaloid); }

private:
	Petaloid*        m_petaloid;
};

Sepaloid::Sepaloid(Sepaloid::Mapping mapping[], int n)
	: m_masterPetaloid(nullptr)
{
	for (int i = 0; i < n; i++) {
		m_mapping.insert(ByteArray(mapping[i].id), &mapping[i]);
	}
}

Petaloid* Sepaloid::registerLocalPetaloid(Petaloid *petaloid, petaloid_dtor_t dtor)
{
	if (petaloid)
		return registerPetaloid(*petaloid, NULL, dtor) ? petaloid : NULL;
	return NULL;
}

Petaloid* Sepaloid::registerPetaloidForPath(const String &path, const char *pname, int arg, char **argv)
{
	if (!FileSystem::exists(path)) {
		addError(_Fr("Petaloid not found by specified path: %s") % path);
		return NULL;
	}

	Dl::Handle ph = Dl::open(path);
	if (ph) {
		petaloid_ctor_t petaloid_ctor = reinterpret_cast<petaloid_ctor_t>(Dl::ptr(ph, CWT_PETALOID_CONSTRUCTOR_NAME));
		petaloid_dtor_t petaloid_dtor = reinterpret_cast<petaloid_dtor_t>(Dl::ptr(ph, CWT_PETALOID_DESTRUCTOR_NAME));

		if (petaloid_ctor) {
			Petaloid *p = reinterpret_cast<Petaloid*>(petaloid_ctor(pname, arg, argv));
			if (p) {
				return registerPetaloid(*p, ph, petaloid_dtor) ? p : NULL;
			} else {
				addError(_Fr("Failed to construct petaloid specified by path: %s") % path);
			}
		} else {
			addError(_Fr("Constructor not found for petaloid specified by path: %s") % path);
		}
		Dl::close(ph);
	} else {
		addError(_Fr("Failed to open petaloid specified by path: %s") % path);
	}
	return NULL;
}

Petaloid* Sepaloid::registerPetaloidForName(const String &name, const char *pname, int arg, char **argv)
{
	String filename = Dl::buildDlFileName(name);

	Vector<String>::const_iterator it    =  m_searchPaths.cbegin();
	Vector<String>::const_iterator itEnd =  m_searchPaths.cend();

	for (; it != itEnd; it++) {
		String path(*it);
		path.append(String(1, FileSystem::separator()));
		path.append(filename);

		Logger::debug(_Fr("Looking for petaloid [%s] at '%s' as '%s'") % name % *it % path);
		if (FileSystem::exists(path)) {
			Logger::debug(_Fr("Petaloid [%s] found at '%s' as '%s'") % name % *it % path);
			return registerPetaloidForPath(path, pname, arg, argv);
		}
	}
	addError(_Fr("Petaloid not found by specified name: %s") % name);
	return NULL;
}

bool Sepaloid::registerPetaloid(Petaloid &petaloid, Dl::Handle ph, petaloid_dtor_t dtor)
{
	int nemitters, ndetectors;
	const EmitterMapping* emitters = petaloid.getEmitters(&nemitters);
	const DetectorMapping* detectors = petaloid.getDetectors(&ndetectors);

	MappingHash::iterator itEnd = m_mapping.end();

	if (emitters) {
		for (int i = 0; i < nemitters; i++) {
			ByteArray emitter_id(emitters[i].id);
			MappingHash::iterator it = m_mapping.find(emitter_id);
			if (it != itEnd) {
				it.value()->map->appendEmitter(reinterpret_cast<Emitter*>(emitters[i].emitter));
			} else {
				Logger::warn(
					_Fr("Emitter '%s' not found while registering petaloid [%s] ...")
					% emitters[i].id
					% petaloid.name());
				Logger::warn(_Tr("... may be signal/slot mapping is not supported for this application"));
			}
		}
	}

	if (detectors) {
		for (int i = 0; i < ndetectors; i++) {
			ByteArray detector_id(detectors[i].id);
			MappingHash::iterator it = m_mapping.find(detector_id);
			if (it != itEnd) {
				it.value()->map->appendDetector(&petaloid, detectors[i].detector);
			} else {
				Logger::warn(
					_Fr("Detector '%s' not found while registering petaloid [%s] ...")
					% emitters[i].id
					% petaloid.name());
				Logger::warn(_Tr("... may be signal/slot mapping is not supported for this application"));
			}
		}
	}

	petaloid.m_sepaloidPtr = this;
	PetaloidSpec pspec(&petaloid, ph, dtor);
	m_petaloids.append(pspec);

	// Petaloid must be run in a separate thread.
	if (petaloid.run) {
		m_threads.append(new PetaloidThreaded(&petaloid));
		Logger::debug(_Fr("Petaloid [%s] registered as threaded") % petaloid.name());
	} else {
		Logger::debug(_Fr("Petaloid [%s] registered") % petaloid.name());
	}

	return true;
}

void Sepaloid::connectAll()
{
	MappingHash::iterator it = m_mapping.begin();
	MappingHash::iterator itEnd = m_mapping.end();

	for(; it != itEnd; it++ ) {
		it.value()->map->connectAll();
	}
}

void Sepaloid::disconnectAll()
{
	MappingHash::iterator it = m_mapping.begin();
	MappingHash::iterator itEnd = m_mapping.end();

	for(; it != itEnd; it++ ) {
		it.value()->map->disconnectAll();
	}
}

void Sepaloid::unregisterAll()
{
	Vector<Thread*>::iterator itThread = m_threads.begin();
	Vector<Thread*>::iterator itThreadEnd = m_threads.end();

	for (; itThread != itThreadEnd; ++itThread) {
		delete *itThread;
	}
	m_threads.clear();

	Vector<PetaloidSpec>::iterator it = m_petaloids.begin();
	Vector<PetaloidSpec>::iterator itEnd = m_petaloids.end();

	for (;it != itEnd; it++) {
		CWT_ASSERT(it->p);
		String pname = it->p->name();
		if (it->dtor) {
			it->dtor(it->p);
			it->p = NULL;
		}
		if (it->ph) {
			Dl::close(it->ph);
		}
		Logger::debug(_Fr("Petaloid [%s] unregistered") % pname);
	}
	m_petaloids.clear();
}

void Sepaloid::start()
{
	Vector<PetaloidSpec>::iterator it = m_petaloids.begin();
	Vector<PetaloidSpec>::iterator itEnd = m_petaloids.end();

	for (;it != itEnd; it++) {
		CWT_ASSERT(it->p);
		it->p->onStart();
	}
}

int Sepaloid::exec()
{
	int r = 0;

	// Exclude master petaloid from threads pool of sepaloid
	if (m_masterPetaloid) {
		size_t size = m_threads.size();
		for (size_t i = 0; i < size; ++i) {
			PetaloidThreaded *pt = dynamic_cast<PetaloidThreaded*>(m_threads[i]);
			if (pt->petaloid() == m_masterPetaloid) {
				m_threads.remove(i);
				delete pt;
				break;
			}
		}
	}

	Vector<Thread*>::iterator itThread = m_threads.begin();
	Vector<Thread*>::iterator itThreadEnd = m_threads.end();

	for (;itThread != itThreadEnd; itThread++) {
		(*itThread)->start();
	}

	if (m_masterPetaloid && m_masterPetaloid->run) {
		r = m_masterPetaloid->run(m_masterPetaloid);
	}

	for (itThread = m_threads.begin(); itThread != itThreadEnd; itThread++) {
		(*itThread)->wait();
	}

	Vector<PetaloidSpec>::iterator itPetaloid = m_petaloids.begin();
	Vector<PetaloidSpec>::iterator itPetaloidEnd = m_petaloids.end();

	for (;itPetaloid != itPetaloidEnd; itPetaloid++) {
		CWT_ASSERT(itPetaloid->p);
		itPetaloid->p->onFinish();
	}

	return r;
}

/*
void Sepaloid::quit()
{
	AutoLock(this);
	Vector<Thread*>::iterator itThread = m_threads.begin();
	Vector<Thread*>::iterator itThreadEnd = m_threads.end();

	for (;itThread != itThreadEnd; itThread++) {
		(*itThread)->quit();
	}
}
*/

CWT_NS_END

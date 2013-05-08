/*
 * sepaloid.cpp
 *
 *  Created on: Feb 9, 2011
 *      Author: wladt
 *  Removed to CWT on: Feb 12, 2013
 */

#include "../include/cwt/logger.hpp"
#include "../include/cwt/sepaloid.hpp"
#include "../include/cwt/filesystem.hpp"
#include "../include/cwt/dl.hpp"

CWT_NS_BEGIN

Sepaloid::Sepaloid(Sepaloid::Mapping mapping[], int n) : m_searchPaths()
{
	for (int i = 0; i < n; i++) {
		m_mapping.insert(ByteArray(mapping[i].id), &mapping[i]);
	}
}

Petaloid* Sepaloid::registerLocalPetaloid(Petaloid &petaloid, petaloid_dtor_t dtor)
{
	return registerPetaloid(petaloid, NULL, dtor) ? &petaloid : NULL;
}

Petaloid* Sepaloid::registerStaticPetaloid(Petaloid &petaloid)
{
	return registerPetaloid(petaloid, NULL, NULL) ? &petaloid : NULL;
}

Petaloid* Sepaloid::registerPetaloidForPath(const String &path, const char *pname, int arg, char **argv)
{
	if (!FileSystem::exists(path)) {
		addError(_Tr("Petaloid not found by specified path: %s"), path.utf16());
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
				addError(_Tr("failed to construct Petaloid specified by path: %ls"), path.utf16());
			}
		} else {
			addError(_Tr("constructor not found for Petaloid specified by path: %ls"), path.utf16());
		}
		Dl::close(ph);
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
		path.append(FileSystem::separator());
		path.append(filename);

		if (FileSystem::exists(path)) {
			Logger::debug(_Tr("Petaloid found at '%ls' by specified name: %ls")
					, it->utf16()
					, name.utf16());
			return registerPetaloidForPath(path, pname, arg, argv);
		}
	}
	addError(_Tr("Petaloid not found by specified name: %ls"), name.utf16());
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
					_Tr("emitter '%s' not found while registering Petaloid '%ls' ...")
					, emitters[i].id
					, petaloid.name().utf16());
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
					_Tr("detector '%s' not found while registering Petaloid '%ls' ...")
					, emitters[i].id
					, petaloid.name().utf16());
				Logger::warn(_Tr("... may be signal/slot mapping is not supported for this application"));
			}
		}
	}

	PetaloidSpec pspec(&petaloid, ph, dtor);
	m_petaloids.append(pspec);
	Logger::debug("Petaloid [%ls] registered.", petaloid.name().utf16());

	return true;
}


void Sepaloid::connectAll()
{
	MappingHash::iterator it = m_mapping.begin();
	MappingHash::iterator itEnd = m_mapping.end();

	for(; it != itEnd; it++ ) {
		it.value()->map->connectAll();
	}

	/* initialize all petaloids */
/*
	Vector<PetaloidSpec>::iterator p = m_petaloids.begin();
	Vector<PetaloidSpec>::iterator pEnd = m_petaloids.end();

	for (;p != pEnd; p++) {
		CWT_ASSERT(p->p);
		p->p->init();
	}
*/
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
	Vector<PetaloidSpec>::iterator it = m_petaloids.begin();
	Vector<PetaloidSpec>::iterator itEnd = m_petaloids.end();

	for (;it != itEnd; it++) {
		CWT_ASSERT(it->p);
		if (it->dtor) {
			it->dtor(it->p);
			it->p = NULL;
		}
		if (it->ph) {
			Dl::close(it->ph);
		}
	}

	m_petaloids.clear();
}

CWT_NS_END

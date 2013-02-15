/*
 * sepaloid.cpp
 *
 *  Created on: Feb 9, 2011
 *      Author: wladt
 *  Removed to CWT on: Feb 12, 2013
 */

#include <cwt/sepaloid.hpp>
#include <cwt/logger.hpp>
#include <cwt/filesystem.hpp>
#include <cwt/dl.hpp>

CWT_NS_BEGIN

Sepaloid::Sepaloid(Sepaloid::Mapping mapping[], int n) : m_searchPaths()
{
	for (int i = 0; i < n; i++) {
		m_mapping.insert(ByteArray(mapping[i].id), &mapping[i]);
	}
}

bool Sepaloid::registerLocalPetaloid(Petaloid &petaloid, __petaloid_dtor__ dtor)
{
	return registerPetaloid(petaloid, NULL, dtor);
}

bool Sepaloid::registerStaticPetaloid(Petaloid &petaloid)
{
	return registerPetaloid(petaloid, NULL, NULL);
}

bool Sepaloid::registerPetaloidForPath(const String &path)
{
	if (!FileSystem::exists(path)) {
		Logger::error(_Tr("Petaloid not found by specified path: %ls"), path.utf16());
		return false;
	}

	Dl::Handle ph = Dl::open(path);
	if (ph) {
		__petaloid_ctor__ petaloid_ctor = reinterpret_cast<__petaloid_ctor__>(Dl::ptr(ph, CWT_PETALOID_CONSTRUCTOR_NAME));
		__petaloid_dtor__ petaloid_dtor = reinterpret_cast<__petaloid_dtor__>(Dl::ptr(ph, CWT_PETALOID_DESTRUCTOR_NAME));

		if (petaloid_ctor) {
			Petaloid *p = reinterpret_cast<Petaloid*>(petaloid_ctor());
			if (p) {
				return registerPetaloid(*p, ph, petaloid_dtor);
			} else {
				Logger::error(_Tr("failed to construct Petaloid specified by path: %ls"), path.utf16());
			}
		} else {
			Logger::error(_Tr("constructor not found for Petaloid specified by path: %ls"), path.utf16());
		}
		Dl::close(ph);
	}
	return false;
}

bool Sepaloid::registerPetaloidForName(const String &name)
{
	String filename = Dl::buildDlFileName(name);

	Vector<String>::const_iterator it    =  m_searchPaths.begin();
	Vector<String>::const_iterator itEnd =  m_searchPaths.end();

	for (; it != itEnd; it++) {
		String path(*it);
		path.append(FileSystem::separator());
		path.append(filename);

		if (FileSystem::exists(path)) {
			Logger::debug(_Tr("Petaloid found at '%ls' by specified name: %ls")
					, it->utf16()
					, name.utf16());
			return registerPetaloidForPath(path);
		}
	}
	Logger::error(_Tr("Petaloid not found by specified name: %ls"), name.utf16());
	return false;
}

bool Sepaloid::registerPetaloid(Petaloid &petaloid, Dl::Handle ph, __petaloid_dtor__ dtor)
{
	int nemitters, ndetectors;
	const EmitterMapping* emitters = petaloid.getEmitters(&nemitters);
	const DetectorMapping* detectors = petaloid.getDetectors(&ndetectors);

	MappingHash::iterator itEnd = m_mapping.end();

	if (emitters) {
		ByteArray emitter_id;

		for (int i = 0; i < nemitters; i++) {
			emitter_id.setRawData(emitters[i].id, strlen(emitters[i].id));
			MappingHash::iterator it = m_mapping.find(emitter_id);
			if (it != itEnd) {
				it.value()->map->appendEmitter(reinterpret_cast<Emitter*>(emitters[i].emitter));
			} else {
				Logger::warn(
					_Tr("emitter '%s' not found while registering Petaloid '%s' ...")
					, emitters[i].id
					, petaloid.name());
				Logger::warn(_Tr("... may be signal/slot mapping is not supported for this application"));
			}
		}
	}

	if (detectors) {
		ByteArray detector_id;

		for (int i = 0; i < ndetectors; i++) {
			detector_id.setRawData(detectors[i].id, strlen(detectors[i].id));
			MappingHash::iterator it = m_mapping.find(detector_id);
			if (it != itEnd) {
				it.value()->map->appendDetector(&petaloid, detectors[i].detector);
			} else {
				Logger::warn(
					_Tr("detector '%s' not found while registering Petaloid '%s' ...")
					, emitters[i].id
					, petaloid.name());
				Logger::warn(_Tr("... may be signal/slot mapping is not supported for this application"));
			}
		}
	}

	PetaloidSpec pspec(&petaloid, ph, dtor);
	m_petaloids.append(pspec);
	Logger::debug("Petaloid [%s] registered.", petaloid.name());

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

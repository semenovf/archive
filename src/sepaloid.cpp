/*
 * sepaloid.cpp
 *
 *  Created on: Feb 9, 2011
 *      Author: wladt
 *  Removed to CWT on: Feb 12, 2013
 */

#include <cwt/sepaloid.hpp>
#include <cwt/logger.hpp>

CWT_NS_BEGIN

Sepaloid::Sepaloid(Sepaloid::Mapping mapping[], int n) : m_searchDirs()
{
	for (int i = 0; i < n; i++) {
		m_mapping.insert(mapping[i].id, &mapping[i]);
	}
}

bool Sepaloid::registerLocalPetaloid(Petaloid &petaloid)
{
	int nemitters, ndetectors;
	const EmitterMapping* emitters = petaloid.getEmitters(&nemitters);
	const DetectorMapping* detectors = petaloid.getDetectors(&ndetectors);

	MappingHash::iterator itEnd = m_mapping.end();

	if (emitters) {
		for (int i = 0; i < nemitters; i++) {
			MappingHash::iterator it = m_mapping.find(emitters[i].id);
			if (it != itEnd) {
				it.value()->map->appendEmitter(reinterpret_cast<Emitter*>(emitters[i].emitter));
			} else {
				Logger::warn(
					_Tr("emitter '%s' not found while registering petaloid '%s' ...")
					, emitters[i].id
					, petaloid.name());
				Logger::warn(_Tr("... may be signal/slot mapping is not supported for this application"));
			}
		}
	}

	if (detectors) {
		for (int i = 0; i < ndetectors; i++) {
			MappingHash::iterator it = m_mapping.find(detectors[i].id);
			if (it != itEnd) {
				it.value()->map->appendDetector(&petaloid, detectors[i].detector);
			} else {
				Logger::warn(
					_Tr("detector '%s' not found while registering petaloid '%s' ...")
					, emitters[i].id
					, petaloid.name());
				Logger::warn(_Tr("... may be signal/slot mapping is not supported for this application"));
			}
		}
	}

	return false;
}


void Sepaloid::connectAll()
{
	Hash<const char*, Mapping*>::iterator it = m_mapping.begin();
	Hash<const char*, Mapping*>::iterator itEnd = m_mapping.end();

	for(; it != itEnd; it++ ) {
		/*JQ_DEBUGF(_Tr("Connecting signal: %s"), it->first.c_str());*/
		it.value()->map->connectAll();
	}
}


CWT_NS_END

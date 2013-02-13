/*
 * petaloid.hpp
 *
 *  Created on: Feb 11, 2011
 *      Author: wladt
 *  Removed to CWT on: Feb 12, 2013
 */

#ifndef __CWT_PETALOID_HPP__
#define __CWT_PETALOID_HPP__

#include <cwt/sigslot.hpp>

/*
#define PETALOID_SLOT_CAST(slotPtr) reinterpret_cast<jq::detector_slot_t>(slotPtr)

#define JQ_DEF_PETALOID_CLASS_FACTORY(PetaloidClass)                           \
	JQ_DEF_DL_CLASS_FACTORY                                                    \
                                                                               \
	void* DlClassFactoryPrivate::newClassInstance( const char_type* ) {        \
		return new PetaloidClass;                                              \
    }                                                                          \
    void DlClassFactoryPrivate::deleteClassInstance( void *ptr )               \
    {                                                                          \
		delete reinterpret_cast<PetaloidClass*>(ptr);                          \
    }



#define sigids_petaloidRegisteredSys _T("$sigPetaloidRegisteredSys$")
#define sigids_petaloidRegistered    _T("$sigPetaloidRegistered$")
#define sigids_petaloidDestroy       _T("$sigPetaloidDestroy$")
*/

CWT_NS_BEGIN

#define DETECTOR_CAST(slot) reinterpret_cast<cwt::Detector>(&slot)
#define EMITTER_CAST(e)     reinterpret_cast<void *>(&e)

class Petaloid;
typedef signal1<void*> Emitter;
typedef void (Petaloid::*Detector)(void*);
typedef struct { const char *id; void *emitter; } EmitterMapping;
typedef struct { const char *id; Detector  detector; } DetectorMapping;

class Petaloid : public has_slots<>
{
private:
	Petaloid() : m_name(NULL) {}

public:
	Petaloid(const char *name) : m_name(name) {}
	virtual ~Petaloid() {}
	const char *name() const { return m_name; }

	virtual const EmitterMapping* getEmitters(int *count) = 0;
	virtual const DetectorMapping* getDetectors(int *count) = 0;
/*
	virtual void onRegistered() {;}
	virtual void onDestroy() { name(); }
*/

public /*slots*/:
/*
	void onRegisteredHelper();
	void onDestroyHelper();
*/
	/*void onRegisteredSys();*/

protected:
/*
	bool registerEmitter( sigids_t sid, void *em );
	bool registerDetector( sigids_t sid, detector_slot_t ds );
*/

protected:
/*
	emitter_map_t  m_emap;
	detector_map_t m_dmap;
*/
private:
	const char *m_name;
};

struct DetectorPair
{
	DetectorPair() : petaloid(NULL), detector(NULL) {}
	DetectorPair(Petaloid *p, Detector d) : petaloid(p), detector(d) {}
	Petaloid *petaloid;
	Detector detector;
};

CWT_NS_END

#endif /* __CWT_PETALOID_HPP__ */

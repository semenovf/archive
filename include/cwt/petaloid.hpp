/*
 * petaloid.hpp
 *
 *  Created on: Feb 11, 2011
 *      Author: wladt
 *  Removed to CWT on: Feb 12, 2013
 */

#ifndef __CWT_PETALOID_HPP__
#define __CWT_PETALOID_HPP__

#include <cwt/cwt.h>
#include <cwt/sigslot.hpp>
#include <cwt/uuid.hpp>

CWT_NS_BEGIN

#define DETECTOR_CAST(slot) reinterpret_cast<cwt::Detector>(&slot)
#define EMITTER_CAST(e)     reinterpret_cast<void *>(&e)

class Sepaloid;
class Petaloid;
typedef signal1<void*> Emitter;
typedef void (Petaloid::*Detector)(void*);
typedef struct { const char *id; void *emitter; } EmitterMapping;
typedef struct { const char *id; Detector  detector; } DetectorMapping;

#define CWT_PETALOID_API extern "C" DLL_API
#define CWT_PETALOID_CONSTRUCTOR_NAME "__petaloid_ctor__"
#define CWT_PETALOID_DESTRUCTOR_NAME "__petaloid_dtor__"
typedef Petaloid* (*petaloid_ctor_t)(/*Sepaloid *sepaloid, */const char *name, int argc, char **argv);
typedef void  (*petaloid_dtor_t)(Petaloid*);


class DLL_API Petaloid : public has_slots<>
{
private:
	Petaloid() : m_name(NULL), m_uuid(), m_sepaloidPtr(nullptr) {}

public:
	Petaloid(const char *name) : m_name(String().fromUtf8(name)), m_uuid(), m_sepaloidPtr(nullptr), run(nullptr) {}
	Petaloid(const char *name, const uuid_t &uuid) : m_name(String().fromUtf8(name)), m_uuid(uuid), m_sepaloidPtr(nullptr), run(nullptr) {}
	Petaloid(const char *name, const Uuid &uuid) : m_name(String().fromUtf8(name)), m_uuid(uuid), m_sepaloidPtr(nullptr), run(nullptr) {}
	virtual ~Petaloid() {}
	const String& name() const { return m_name; }
	const uuid_t& uuid() const { return m_uuid.uuid(); }
	Sepaloid* sepaloid() const { return m_sepaloidPtr; }

	bool isRegistered() const { return m_sepaloidPtr != nullptr ? true : false; }

	static void defaultDtor(Petaloid *p) { CWT_ASSERT(p); delete p; }

	virtual const EmitterMapping* getEmitters(int *count)   { CWT_ASSERT(count); *count = 0; return 0; }
	virtual const DetectorMapping* getDetectors(int *count) { CWT_ASSERT(count); *count = 0; return 0; }

	virtual void onStart() {}  // call from Sepaloid::start()
	virtual void onFinish() {} // call from Sepaloid::finish()

private:
	String    m_name;
	Uuid      m_uuid;
	Sepaloid* m_sepaloidPtr;

public:
	int (*run)(Petaloid*);

	friend class Sepaloid;
};

struct DetectorPair
{
	DetectorPair() : petaloid(NULL), detector(NULL) {}
	DetectorPair(Petaloid *p, Detector d) : petaloid(p), detector(d) {}
	Petaloid *petaloid;
	Detector detector;
};

CWT_NS_END


#define CWT_PETALOID_EMITTER(id, em) { id , EMITTER_CAST(em) }
#define CWT_PETALOID_DETECTOR(id, dt) { id , DETECTOR_CAST(dt) }

#define CWT_PETALOID_EMITTERS_BEGIN                                 \
const EmitterMapping* getEmitters(int *count)                       \
{                                                                   \
	static EmitterMapping emitter_mapping[] = {

#define CWT_PETALOID_EMITTERS_END                                   \
	};                                                              \
	*count = sizeof(emitter_mapping)/sizeof(emitter_mapping[0]) ;   \
	return &emitter_mapping[0];                                     \
}


#define CWT_PETALOID_DETECTORS_BEGIN                                \
const DetectorMapping* getDetectors(int *count)                     \
{                                                                   \
	static DetectorMapping detector_mapping[] = {

#define CWT_PETALOID_DETECTORS_END		                            \
	};                                                              \
	*count = sizeof(detector_mapping)/sizeof(detector_mapping[0]) ; \
	return &detector_mapping[0];                                    \
}

#endif /* __CWT_PETALOID_HPP__ */

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
typedef Petaloid* (*petaloid_ctor_t)(Sepaloid *sepaloid, const char *name, int argc, char **argv);
typedef void  (*petaloid_dtor_t)(Petaloid*);


class DLL_API Petaloid : public has_slots<>
{
private:
	Petaloid() : m_name(NULL), m_uuid() {}

public:
	Petaloid(const char *name) : m_name(String().fromUtf8(name)), m_uuid() {}
	Petaloid(const char *name, const uuid_t &uuid) : m_name(String().fromUtf8(name)), m_uuid(uuid) {}
	Petaloid(const char *name, const Uuid &uuid) : m_name(String().fromUtf8(name)), m_uuid(uuid) {}
	virtual ~Petaloid() {}
	const String& name() const { return m_name; }
	const uuid_t& uuid() const { return m_uuid.uuid(); }

	static void defaultDtor(Petaloid *p) { CWT_ASSERT(p); delete p; }

	virtual const EmitterMapping* getEmitters(int *count)   { CWT_ASSERT(count); *count = 0; return 0; }
	virtual const DetectorMapping* getDetectors(int *count) { CWT_ASSERT(count); *count = 0; return 0; }

private:
	String m_name;
	Uuid m_uuid;
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

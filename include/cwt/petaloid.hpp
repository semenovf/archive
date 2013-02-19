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

CWT_NS_BEGIN

#define DETECTOR_CAST(slot) reinterpret_cast<cwt::Detector>(&slot)
#define EMITTER_CAST(e)     reinterpret_cast<void *>(&e)

class Petaloid;
typedef signal1<void*> Emitter;
typedef void (Petaloid::*Detector)(void*);
typedef struct { const char *id; void *emitter; } EmitterMapping;
typedef struct { const char *id; Detector  detector; } DetectorMapping;

#define CWT_PETALOID_API extern "C" DLL_API
#define CWT_PETALOID_CONSTRUCTOR_NAME "__petaloid_ctor__"
#define CWT_PETALOID_DESTRUCTOR_NAME "__petaloid_dtor__"
typedef Petaloid* (*petaloid_ctor_t)(const char *name, int argc, char **argv);
typedef void  (*petaloid_dtor_t)(Petaloid*);


class Petaloid : public has_slots<>
{
private:
	Petaloid() : m_name(NULL) {}

public:
	Petaloid(const char *name) : m_name(name) {}
	virtual ~Petaloid() {}
	const char *name() const { return m_name; }

	static void defaultDtor(Petaloid *p) { CWT_ASSERT(p); delete p; }

	virtual const EmitterMapping* getEmitters(int *count) = 0;
	virtual const DetectorMapping* getDetectors(int *count) = 0;
	virtual void init() {}

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

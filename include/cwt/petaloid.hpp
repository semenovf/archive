/*
 * petaloid.hpp
 *
 *  Created on: Feb 11, 2011
 *      Author: wladt
 *  Removed to CWT on: Feb 12, 2013
 */

#ifndef __CWT_PETALOID_HPP__
#define __CWT_PETALOID_HPP__

#include <pfs/string.hpp>
#include <cwt/sigslot.hpp>

namespace cwt {

#define DETECTOR_CAST(slot) reinterpret_cast<cwt::detector>(& slot)
#define EMITTER_CAST(e)     reinterpret_cast<void *>(& e)

class sepaloid;
class petaloid;
typedef signal1<void *> emitter;
typedef void (petaloid::* detector)(void *);
typedef struct { int _id; void * _emitter; } emitter_mapping;
typedef struct { int _id; detector _detector; } detector_mapping;

#define CWT_PETALOID_API extern "C" DLL_API
#define CWT_PETALOID_CONSTRUCTOR_NAME "__petaloid_ctor__"
#define CWT_PETALOID_DESTRUCTOR_NAME "__petaloid_dtor__"
typedef petaloid* (*petaloid_ctor_t)(const char * name, int argc, char ** argv);
typedef void  (*petaloid_dtor_t)(petaloid*);


class DLL_API petaloid : public has_slots<>
{
private:
	petaloid() : _name(nullptr), _sepaloidPtr(nullptr), run(nullptr) {}

public:
	petaloid (const char * name) : _name(_u8(name)), _sepaloidPtr(nullptr), run(nullptr) {}
	petaloid (const pfs::string & name) : _name(name), _sepaloidPtr(nullptr), run(nullptr) {}
	virtual ~petaloid() {}

	const pfs::string & name() const { return _name; }
	bool isRegistered () const { return _sepaloidPtr != nullptr ? true : false; }

	virtual const emitter_mapping * getEmitters (int * count)   { PFS_ASSERT(count); *count = 0; return 0; }
	virtual const detector_mapping * getDetectors (int * count) { PFS_ASSERT(count); *count = 0; return 0; }

	virtual bool onStart () { return true; }  // call from sepaloid::start()
	virtual bool onFinish () { return true; } // call from sepaloid::finish()

	static void defaultDtor (petaloid * p) { PFS_ASSERT(p); delete p; }

public: /*signal*/
	cwt::signal2<const pfs::string &, bool &> petaloidRegistered;

private:
	pfs::string _name;
	sepaloid *  _sepaloidPtr;

public:
	int (* run) (petaloid *);

	friend class sepaloid;
};

struct detector_pair
{
	detector_pair () : _petaloid(NULL), _detector(NULL) {}
	detector_pair (petaloid * p, detector d) : _petaloid(p), _detector(d) {}
	petaloid * _petaloid;
	detector   _detector;
};

} // cwt

#define CWT_PETALOID_EMITTER(id, em) { id , EMITTER_CAST(em) }
#define CWT_PETALOID_DETECTOR(id, dt) { id , DETECTOR_CAST(dt) }

#define CWT_PETALOID_EMITTERS_BEGIN                                 \
const cwt::emitter_mapping * getEmitters (int *count)               \
{                                                                   \
	static cwt::emitter_mapping __emitter_mapping[] = {

#define CWT_PETALOID_EMITTERS_END                                   \
	};                                                              \
	*count = sizeof(__emitter_mapping)/sizeof(__emitter_mapping[0]) ;   \
	return & __emitter_mapping[0];                                  \
}


#define CWT_PETALOID_DETECTORS_BEGIN                                \
const cwt::detector_mapping * getDetectors(int *count)               \
{                                                                   \
	static cwt::detector_mapping __detector_mapping[] = {

#define CWT_PETALOID_DETECTORS_END		                            \
	};                                                              \
	*count = sizeof(__detector_mapping)/sizeof(__detector_mapping[0]) ; \
	return & __detector_mapping[0];                                 \
}

#endif /* __CWT_PETALOID_HPP__ */

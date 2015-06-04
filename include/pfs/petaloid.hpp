/*
 * petaloid.hpp
 *
 *  Created on: Feb 11, 2011
 *      Author: wladt
 *  Removed to CWT on: Feb 12, 2013
 */

#ifndef __PFS_PETALOID_HPP__
#define __PFS_PETALOID_HPP__

#include <pfs/string.hpp>
#include <pfs/sigslot.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

#define DETECTOR_CAST(slot) reinterpret_cast<pfs::detector>(& slot)
#define EMITTER_CAST(e)     reinterpret_cast<void *>(& e)

class sepaloid;
class petaloid;
typedef signal1<void *> emitter;
typedef void (petaloid::* detector)(void *);
typedef struct { int _id; void * _emitter; } emitter_mapping;
typedef struct { int _id; detector _detector; } detector_mapping;

#define PFS_PETALOID_API extern "C" DLL_API
#define PFS_PETALOID_CTOR_NAME "__petaloid_ctor__"
#define PFS_PETALOID_DTOR_NAME "__petaloid_dtor__"
typedef petaloid* (*petaloid_ctor_t)(const char * name, int argc, char ** argv);
typedef void  (*petaloid_dtor_t)(petaloid*);


class DLL_API petaloid : public has_slots<>
{
private:
	petaloid() : _name(nullptr), _sepaloidPtr(nullptr), run(nullptr) {}

public:
	petaloid (const char * name) : _name(_u8(name)), _sepaloidPtr(nullptr), run(nullptr) {}
	petaloid (const string & name) : _name(name), _sepaloidPtr(nullptr), run(nullptr) {}
	virtual ~petaloid() {}

	const string & name() const { return _name; }
	bool isRegistered () const { return _sepaloidPtr != nullptr ? true : false; }

	virtual const emitter_mapping * getEmitters (int * count)   { PFS_ASSERT(count); *count = 0; return 0; }
	virtual const detector_mapping * getDetectors (int * count) { PFS_ASSERT(count); *count = 0; return 0; }

	virtual bool onStart () { return true; }  // call from sepaloid::start()
	virtual bool onFinish () { return true; } // call from sepaloid::finish()

	static void defaultDtor (petaloid * p) { PFS_ASSERT(p); delete p; }

public: /*signal*/
	signal2<const string &, bool &> petaloidRegistered;

private:
	string _name;
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

} // pfs

#define PFS_PETALOID_EMITTER(id, em) { id , EMITTER_CAST(em) }
#define PFS_PETALOID_DETECTOR(id, dt) { id , DETECTOR_CAST(dt) }

#define PFS_PETALOID_EMITTERS_BEGIN                                 \
const pfs::emitter_mapping * getEmitters (int *count)               \
{                                                                   \
	static pfs::emitter_mapping __emitter_mapping[] = {

#define PFS_PETALOID_EMITTERS_END                                   \
	};                                                              \
	*count = sizeof(__emitter_mapping)/sizeof(__emitter_mapping[0]) ;   \
	return & __emitter_mapping[0];                                  \
}


#define PFS_PETALOID_DETECTORS_BEGIN                                \
const pfs::detector_mapping * getDetectors(int *count)               \
{                                                                   \
	static pfs::detector_mapping __detector_mapping[] = {

#define PFS_PETALOID_DETECTORS_END		                            \
	};                                                              \
	*count = sizeof(__detector_mapping)/sizeof(__detector_mapping[0]) ; \
	return & __detector_mapping[0];                                 \
}

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_PETALOID_HPP__ */

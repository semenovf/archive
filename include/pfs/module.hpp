/*
 * module.hpp
 *
 *  Created on: Feb 11, 2011
 *      Author: wladt
 */

#ifndef __PFS_MODULE_HPP__
#define __PFS_MODULE_HPP__

#include <pfs/string.hpp>
#include <pfs/sigslot.hpp>
#include <pfs/notification.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

#define DETECTOR_CAST(slot) reinterpret_cast<pfs::detector>(& slot)
#define EMITTER_CAST(e)     reinterpret_cast<void *>(& e)

class dispatcher;
class module;
typedef signal1<void *> emitter;
typedef void (module::* detector)(void *);
typedef struct { int _id; void * _emitter; } emitter_mapping;
typedef struct { int _id; detector _detector; } detector_mapping;

#define PFS_MODULE_API extern "C" DLL_API
#define PFS_MODULE_CTOR_NAME "__module_ctor__"
#define PFS_MODULE_DTOR_NAME "__module_dtor__"
typedef module * (* module_ctor_t)(const char * name, void *);
typedef void  (* module_dtor_t)(module *);


class DLL_API module : public has_slots<>
{
	friend class dispatcher;

public: /*signal*/
	signal2<const string &, bool &> emit_module_registered;

private:
	string       _name;
	dispatcher * _pdispatcher;

public:
	int (* run) (module *); // TODO declare it as `int (module::*) ()'

protected:
	module ()
		: _pdispatcher()
		, run(0)
	{}

	void set_name (const string & name)
	{
		_name = name;
	}

//	module (const string & name)
//		: _name(name)
//		, _pdispatcher(0)
//		, run(0)
//	{}
//
//	module (const string & name, int (* loop) (module *))
//		: _name(name)
//		, _pdispatcher(0)
//		, run(loop)
//	{}

public:
	virtual ~module() {}

	const string & name() const
	{
		return _name;
	}

	bool is_registered () const
	{
		return _pdispatcher != 0 ? true : false;
	}

	void set_dispatcher (dispatcher * pdispatcher)
	{
		_pdispatcher = pdispatcher;
	}

	virtual const emitter_mapping * get_emitters (int * count)
	{
		PFS_ASSERT(count);
		*count = 0;
		return 0;
	}

	virtual const detector_mapping * get_detectors (int * count)
	{
		PFS_ASSERT(count);
		*count = 0;
		return 0;
	}

	/**
	 * @brief Module's onStart() method called after loaded and connection completed.
	 */
	virtual bool on_start (notification & nx)
	{
		return true;
	}

	virtual bool on_finish ()
	{
		return true;
	}

//	static void defaultDtor (module * p)
//	{
//		PFS_ASSERT(p);
//		delete p;
//	}
};

struct detector_pair
{
	detector_pair () : _module(NULL), _detector(NULL) {}
	detector_pair (module * p, detector d) : _module(p), _detector(d) {}
	module * _module;
	detector _detector;
};

} // pfs

#define PFS_MODULE_EMITTER(id, em) { id , EMITTER_CAST(em) }
#define PFS_MODULE_DETECTOR(id, dt) { id , DETECTOR_CAST(dt) }

#define PFS_MODULE_EMITTERS_BEGIN                                       \
const pfs::emitter_mapping * get_emitters (int *count)                  \
{                                                                       \
	static pfs::emitter_mapping __emitter_mapping[] = {

#define PFS_MODULE_EMITTERS_END                                         \
	};                                                                  \
	*count = sizeof(__emitter_mapping)/sizeof(__emitter_mapping[0]) ;   \
	return & __emitter_mapping[0];                                      \
}

#define PFS_MODULE_DETECTORS_BEGIN                                      \
const pfs::detector_mapping * get_detectors(int *count)                 \
{                                                                       \
	static pfs::detector_mapping __detector_mapping[] = {

#define PFS_MODULE_DETECTORS_END		                                \
	};                                                                  \
	*count = sizeof(__detector_mapping)/sizeof(__detector_mapping[0]) ; \
	return & __detector_mapping[0];                                     \
}

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_MODULE_HPP__ */

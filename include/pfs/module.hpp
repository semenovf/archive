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
#include <pfs/atomic.hpp>

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

public:
	struct log_consumer : public has_slots<>
	{
		friend class module;
		virtual void on_info  (const string &) {}
		virtual void on_debug (const string &) {}
		virtual void on_warn  (const string &) {}
		virtual void on_error (const string &) {}

	private:
		void _on_info  (const string & s) { on_info(s);  }
		void _on_debug (const string & s) { on_debug(s); }
		void _on_warn  (const string & s) { on_warn(s);  }
		void _on_error (const string & s) { on_error(s); }
	};

public: // signals
	signal2<const string &, bool &> emit_module_registered;

	signal0<>               emit_quit;
	signal1<const string &> emit_info;
	signal1<const string &> emit_debug;
	signal1<const string &> emit_warn;
	signal1<const string &> emit_error;

private:
	string       _name;
	dispatcher * _pdispatcher;
	atomic_int   _quitfl; // quit flag

public:
	int (* run) (module *); // TODO declare it as `int (module::*) ()'

protected:
	module ()
		: _pdispatcher(0)
		, _quitfl(0)
		, run(0)
	{}

	void set_name (const string & name)
	{
		_name = name;
	}

public:
	virtual ~module () {}

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

	dispatcher * get_dispatcher ()
	{
		return _pdispatcher;
	}

	const dispatcher * get_dispatcher () const
	{
		return _pdispatcher;
	}

	void connect_info  (log_consumer * p);
	void connect_debug (log_consumer * p);
	void connect_warn  (log_consumer * p);
	void connect_error (log_consumer * p);

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

	bool is_quit () const
	{
		return _quitfl.load() == 0 ? false : true;
	}

	/**
	 * @brief Module's on_start() method called after loaded and connection completed.
	 */
	virtual bool on_start ()
	{
		return true;
	}

	virtual bool on_finish ()
	{
		return true;
	}

public: // slots
	void on_quit ()
	{
		_quitfl.store(1);
	}
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

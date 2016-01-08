/**
 * @file petaloid.hpp
 * @author
 * @date
 */

#ifndef __IO_PETALOID_HPP__
#define __IO_PETALOID_HPP__

#include <pfs/petaloid.hpp>
#include "api.hpp"

#define __PETALOID io::petaloid::self()

namespace io {

class petaloid : public pfs::petaloid
{
private:
	static petaloid * _self;

public:
	static petaloid * self () { return _self; }

public:
	petaloid () : pfs::petaloid("io")
	{
		PFS_ASSERT(_self == nullptr);
		_self = this;
	}

	virtual bool onStart () override  { return true; }
	virtual bool on_finish () override { return true; }

	PFS_PETALOID_EMITTERS_BEGIN
		  PFS_PETALOID_EMITTER(API_INFO    , emitInfo)
		, PFS_PETALOID_EMITTER(API_DEBUG   , emitDebug)
		, PFS_PETALOID_EMITTER(API_WARN    , emitWarn)
		, PFS_PETALOID_EMITTER(API_ERROR   , emitError)
		, PFS_PETALOID_EMITTER(API_GET_PREF_BOOLEAN, emitGetBooleanPref)
		, PFS_PETALOID_EMITTER(API_GET_PREF_INTEGER, emitGetIntegerPref)
		, PFS_PETALOID_EMITTER(API_GET_PREF_NUMBER , emitGetNumberPref)
		, PFS_PETALOID_EMITTER(API_GET_PREF_STRING , emitGetStringPref)
	PFS_PETALOID_EMITTERS_END

	PFS_PETALOID_DETECTORS_BEGIN
		  PFS_PETALOID_DETECTOR(API_QUIT, petaloid::onQuit)
		, PFS_PETALOID_DETECTOR(API_GUI_READY, petaloid::onGuiReady)
	PFS_PETALOID_DETECTORS_END

public: // signals
	pfs::signal1<const pfs::string &> emitInfo;
	pfs::signal1<const pfs::string &> emitDebug;
	pfs::signal1<const pfs::string &> emitWarn;
	pfs::signal1<const pfs::string &> emitError;
	pfs::signal2<const pfs::string &, bool *>        emitGetBooleanPref;
	pfs::signal2<const pfs::string &, integral_t *>  emitGetIntegerPref;
	pfs::signal2<const pfs::string &, real_t *>      emitGetNumberPref;
	pfs::signal2<const pfs::string &, pfs::string *> emitGetStringPref;

private: // slots
	void onQuit () {}
	void onGuiReady (bool) {}
};

} // io

#endif /* __IO_PETALOID_HPP__ */

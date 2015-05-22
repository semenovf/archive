/**
 * @file petaloid.hpp
 * @author
 * @date
 */

#ifndef __CORE_PETALOID_HPP__
#define __CORE_PETALOID_HPP__

#include <pfs/petaloid.hpp>
#include <pfs/unitype.hpp>
#include "api.hpp"

#define __PETALOID core::petaloid::self()

namespace core {

class petaloid : public pfs::petaloid
{
private:
	static petaloid * _self;

public:
	petaloid * self () { return _self; }

public:
	petaloid () : pfs::petaloid("core")
	{
		PFS_ASSERT(_self == nullptr);
		_self = this;
	}

	virtual bool onStart () override  { return true; }
	virtual bool onFinish () override { return true; }

	PFS_PETALOID_EMITTERS_BEGIN
		  PFS_PETALOID_EMITTER(API_INFO    , emitInfo)
		, PFS_PETALOID_EMITTER(API_DEBUG   , emitDebug)
		, PFS_PETALOID_EMITTER(API_WARN    , emitWarn)
		, PFS_PETALOID_EMITTER(API_ERROR   , emitError)
		, PFS_PETALOID_EMITTER(API_GET_PREF, emitPref)
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
	pfs::signal2<const pfs::string &, pfs::unitype &> emitPref;

private: // slots
	void onQuit () {}
	void onGuiReady (bool) {}
};

} // core

#endif /* __CORE_PETALOID_HPP__ */

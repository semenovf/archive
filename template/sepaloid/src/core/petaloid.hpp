/**
 * @file petaloid.hpp
 *
 *  Created on: Jul 20, 2014
 *      Author: wladt
 */

#ifndef __HG_BOOKER_PETALOID_CORE_HPP__
#define __HG_BOOKER_PETALOID_CORE_HPP__

#include <cwt/petaloid.hpp>
#include <pfs/unitype.hpp>
#include "api.hpp"

#define __PETALOID hg::booker::core::petaloid::self()

namespace hg { namespace booker { namespace core {

class petaloid : public cwt::petaloid
{
private:
	static petaloid * _self;

public:
	petaloid * self () { return _self; }

public:
	petaloid () : cwt::petaloid("hg::booker::core")
	{
		PFS_ASSERT(_self == nullptr);
		_self = this;
	}

	virtual bool onStart () override  { return true; }
	virtual bool onFinish () override { return true; }

	CWT_PETALOID_EMITTERS_BEGIN
		  CWT_PETALOID_EMITTER(API_INFO    , emitInfo)
		, CWT_PETALOID_EMITTER(API_DEBUG   , emitDebug)
		, CWT_PETALOID_EMITTER(API_WARN    , emitWarn)
		, CWT_PETALOID_EMITTER(API_ERROR   , emitError)
	    , CWT_PETALOID_EMITTER(API_GET_PREF, emitPref)
	CWT_PETALOID_EMITTERS_END

	CWT_PETALOID_DETECTORS_BEGIN
	  	  CWT_PETALOID_DETECTOR(API_QUIT, petaloid::onQuit)
		, CWT_PETALOID_DETECTOR(API_GUI_READY, petaloid::onGuiReady)
	CWT_PETALOID_DETECTORS_END

public: // signals
	cwt::signal1<const pfs::string &> emitInfo;
	cwt::signal1<const pfs::string &> emitDebug;
	cwt::signal1<const pfs::string &> emitWarn;
	cwt::signal1<const pfs::string &> emitError;
	cwt::signal2<const pfs::string &, pfs::unitype &> emitPref;

private: // slots
	void onQuit () {}
	void onGuiReady (bool) {}
};

}}} // hg::booker::core

#endif /* __HG_BOOKER_PETALOID_CORE_HPP__ */

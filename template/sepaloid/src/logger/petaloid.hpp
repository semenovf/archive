/**
 * @file petaloid.hpp
 * @author
 * @date
 */

#ifndef __LOGGER_PETALOID_HPP__
#define __LOGGER_PETALOID_HPP__

#include <cwt/petaloid.hpp>
#include <cwt/critical.hpp>
#include <pfs/unitype.hpp>
#include "api.hpp"

#define __PETALOID logger::petaloid::self()

namespace logger {

class petaloid : public cwt::petaloid
{
private:
	static petaloid * _self;

public:
	static petaloid * self () { return _self; }

public:
	petaloid () : cwt::petaloid("logger")
	{
		PFS_ASSERT(_self == nullptr);
		_self = this;
	}

	virtual bool onStart () override  { return true; }
	virtual bool onFinish () override { return true; }

	CWT_PETALOID_EMITTERS_BEGIN
	      CWT_PETALOID_EMITTER(API_GET_PREF, emitPref)
	CWT_PETALOID_EMITTERS_END

	CWT_PETALOID_DETECTORS_BEGIN
		  CWT_PETALOID_DETECTOR(API_QUIT     , petaloid::onQuit)
		, CWT_PETALOID_DETECTOR(API_INFO     , petaloid::onInfo)
		, CWT_PETALOID_DETECTOR(API_DEBUG    , petaloid::onDebug)
		, CWT_PETALOID_DETECTOR(API_WARN     , petaloid::onWarn)
		, CWT_PETALOID_DETECTOR(API_ERROR    , petaloid::onError)
	    , CWT_PETALOID_DETECTOR(API_CRITICAL , petaloid::onCritical)
		, CWT_PETALOID_DETECTOR(API_GUI_READY, petaloid::onGuiReady)

	CWT_PETALOID_DETECTORS_END

public: // signals
	cwt::signal2<const pfs::string &, pfs::unitype &> emitPref;

private: // slots
	void onInfo (const pfs::string &);
	void onDebug (const pfs::string &);
	void onWarn (const pfs::string &);
	void onError (const pfs::string &);
	void onCritical (const cwt::critical &);

	void onQuit () {}
	void onGuiReady (bool) {}
};

} // logger

#endif /* __LOGGER_PETALOID_HPP__ */

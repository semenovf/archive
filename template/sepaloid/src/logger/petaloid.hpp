/**
 * @file petaloid.hpp
 * @author
 * @date
 */

#ifndef __LOGGER_PETALOID_HPP__
#define __LOGGER_PETALOID_HPP__

#include <pfs/petaloid.hpp>
#include <pfs/critical.hpp>
#include "api.hpp"

#define __PETALOID logger::petaloid::self()

namespace logger {

class petaloid : public pfs::petaloid
{
private:
	static petaloid * _self;

public:
	static petaloid * self () { return _self; }

public:
	petaloid () : pfs::petaloid("logger")
	{
		PFS_ASSERT(_self == nullptr);
		_self = this;
	}

	virtual bool onStart () override  { return true; }
	virtual bool on_finish () override { return true; }

	PFS_PETALOID_EMITTERS_BEGIN
		  PFS_PETALOID_EMITTER(API_GET_PREF_BOOLEAN, emitGetBooleanPref)
		, PFS_PETALOID_EMITTER(API_GET_PREF_INTEGER, emitGetIntegerPref)
		, PFS_PETALOID_EMITTER(API_GET_PREF_NUMBER , emitGetNumberPref)
		, PFS_PETALOID_EMITTER(API_GET_PREF_STRING , emitGetStringPref)
	PFS_PETALOID_EMITTERS_END

	PFS_PETALOID_DETECTORS_BEGIN
		  PFS_PETALOID_DETECTOR(API_QUIT     , petaloid::onQuit)
		, PFS_PETALOID_DETECTOR(API_INFO     , petaloid::onInfo)
		, PFS_PETALOID_DETECTOR(API_DEBUG    , petaloid::onDebug)
		, PFS_PETALOID_DETECTOR(API_WARN     , petaloid::onWarn)
		, PFS_PETALOID_DETECTOR(API_ERROR    , petaloid::onError)
	    , PFS_PETALOID_DETECTOR(API_CRITICAL , petaloid::onCritical)
		, PFS_PETALOID_DETECTOR(API_GUI_READY, petaloid::onGuiReady)
	PFS_PETALOID_DETECTORS_END

public: // signals
	pfs::signal2<const pfs::string &, bool *>        emitGetBooleanPref;
	pfs::signal2<const pfs::string &, integral_t *>  emitGetIntegerPref;
	pfs::signal2<const pfs::string &, real_t *>      emitGetNumberPref;
	pfs::signal2<const pfs::string &, pfs::string *> emitGetStringPref;

private: // slots
	void onInfo (const pfs::string &);
	void onDebug (const pfs::string &);
	void onWarn (const pfs::string &);
	void onError (const pfs::string &);
	void onCritical (const pfs::critical &);

	void onQuit () {}
	void onGuiReady (bool) {}
};

} // logger

#endif /* __LOGGER_PETALOID_HPP__ */

/**
 * @file petaloid.hpp
 * @author
 * @date
 */

#ifndef __PREFS_PETALOID_HPP__
#define __PREFS_PETALOID_HPP__

#include <pfs/petaloid.hpp>
#include <pfs/map.hpp>
#include "api.hpp"

#define __PETALOID prefs::petaloid::self()

namespace prefs {

class petaloid : public pfs::petaloid
{
private: // static
	static petaloid * _self;

public:
	static petaloid * self () { return _self; }

public:
	petaloid () : pfs::petaloid("prefs")
	{
		PFS_ASSERT(_self == nullptr);
		_self = this;
	}

	virtual bool onStart () override  { return true; }
	virtual bool on_finish () override { return true; }

	PFS_PETALOID_EMITTERS_BEGIN
		  PFS_PETALOID_EMITTER(API_INFO  , emitInfo)
		, PFS_PETALOID_EMITTER(API_DEBUG , emitDebug)
		, PFS_PETALOID_EMITTER(API_WARN  , emitWarn)
		, PFS_PETALOID_EMITTER(API_ERROR , emitError)
	PFS_PETALOID_EMITTERS_END

	PFS_PETALOID_DETECTORS_BEGIN
		  PFS_PETALOID_DETECTOR(API_QUIT, petaloid::onQuit)
		, PFS_PETALOID_DETECTOR(API_GUI_READY, petaloid::onGuiReady)
		, PFS_PETALOID_DETECTOR(API_GET_PREF_BOOLEAN, petaloid::onGetBooleanPref)
		, PFS_PETALOID_DETECTOR(API_GET_PREF_INTEGER, petaloid::onGetIntegerPref)
		, PFS_PETALOID_DETECTOR(API_GET_PREF_NUMBER , petaloid::onGetNumberPref)
		, PFS_PETALOID_DETECTOR(API_GET_PREF_STRING , petaloid::onGetStringPref)
	PFS_PETALOID_DETECTORS_END

public: // signals
	pfs::signal1<const pfs::string &> emitInfo;
	pfs::signal1<const pfs::string &> emitDebug;
	pfs::signal1<const pfs::string &> emitWarn;
	pfs::signal1<const pfs::string &> emitError;

private: // slots
	void onQuit () {}
	void onGuiReady (bool) {}

	void onGetBooleanPref (const pfs::string &, bool *) {}
	void onGetIntegerPref (const pfs::string &, integral_t *) {}
	void onGetNumberPref  (const pfs::string &, real_t *) {}
	void onGetStringPref (const pfs::string &, pfs::string *) {}

private:
//	pfs::unitype pref (const pfs::string & key) const;
//	void setPref (const pfs::string & key, const pfs::unitype & value);
//	void setPref (const pfs::string & key, const pfs::string & value) { setPref(key, pfs::unitype(value)); }
};

} // prefs

#endif /* __PREFS_PETALOID_HPP__ */

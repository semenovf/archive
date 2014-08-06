/**
 * @file petaloid.hpp
 * @author
 * @date
 */

#ifndef __PREFS_PETALOID_HPP__
#define __PREFS_PETALOID_HPP__

#include <cwt/petaloid.hpp>
#include <pfs/unitype.hpp>
#include <pfs/map.hpp>
#include "api.hpp"

#define __PETALOID prefs::petaloid::self()

namespace prefs {

class petaloid : public cwt::petaloid
{
	typedef pfs::map<pfs::string, pfs::unitype> prefs_map_type;

private: // static
	static petaloid * _self;

private:
	prefs_map_type    _prefs;

public:
	static petaloid * self () { return _self; }

public:
	petaloid () : cwt::petaloid("prefs")
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
	CWT_PETALOID_EMITTERS_END

	CWT_PETALOID_DETECTORS_BEGIN
		  CWT_PETALOID_DETECTOR(API_QUIT, petaloid::onQuit)
		, CWT_PETALOID_DETECTOR(API_GUI_READY, petaloid::onGuiReady)
		, CWT_PETALOID_DETECTOR(API_GET_PREF , petaloid::onGetPref)
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
	void onGetPref (const pfs::string & key, pfs::unitype & value) { value = pref(key); }

private:
	pfs::unitype pref (const pfs::string & key) const;
	void setPref (const pfs::string & key, const pfs::unitype & value);
	void setPref (const pfs::string & key, const pfs::string & value) { setPref(key, pfs::unitype(value)); }
};

} // prefs

#endif /* __PREFS_PETALOID_HPP__ */

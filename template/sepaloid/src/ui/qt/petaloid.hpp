/**
 * @file petaloid.hpp
 * @author
 * @date
 */

#ifndef __UI_QT_PETALOID_HPP__
#define __UI_QT_PETALOID_HPP__

#include <pfs/petaloid.hpp>
#include <pfs/platform.hpp>
#include <pfs/critical.hpp>
#include "api.hpp"
#include <QObject>
#include <QString>
#include <QMetaType>

class MainWindow;

#define __PETALOID ui::petaloid::self()

namespace ui {
class petaloid;
}

class QtAdapter : public QObject
{
	Q_OBJECT

public:
	QtAdapter ();

public Q_SLOTS:
	void onCritical (const pfs::critical & critical);

Q_SIGNALS:
	void emitCritical   (const pfs::critical & critical);
	void emitInfo       (const pfs::datetime & dt, const pfs::string & msg);
	void emitDebug      (const pfs::datetime & dt, const pfs::string & msg);
	void emitWarn       (const pfs::datetime & dt, const pfs::string & msg);
	void emitError      (const pfs::datetime & dt, const pfs::string & msg);

	friend class ui::petaloid;
};


namespace ui {

class petaloid : public pfs::petaloid
{
private:
	double      _hppmm;
	double      _vppmm;

public:
	int     _argc;
	char ** _argv;
	bool    _guiReady;
	MainWindow * _mainWindow;

	QtAdapter qt;

private:
	static petaloid * _self;

public:
	static petaloid * self () { return _self; }

public:
	petaloid (int argc, char * argv[]);
	virtual bool onStart ();
	virtual bool on_finish ();

	PFS_PETALOID_EMITTERS_BEGIN
	      PFS_PETALOID_EMITTER(API_QUIT      , emitQuit)
	    , PFS_PETALOID_EMITTER(API_CRITICAL  , emitCritical)
	    , PFS_PETALOID_EMITTER(API_INFO      , emitInfo)
	    , PFS_PETALOID_EMITTER(API_DEBUG     , emitDebug)
	    , PFS_PETALOID_EMITTER(API_WARN      , emitWarn)
	    , PFS_PETALOID_EMITTER(API_ERROR     , emitError)
	    , PFS_PETALOID_EMITTER(API_GUI_READY , emitGuiReady)
		, PFS_PETALOID_EMITTER(API_GET_PREF_BOOLEAN, emitGetBooleanPref)
		, PFS_PETALOID_EMITTER(API_GET_PREF_INTEGER, emitGetIntegerPref)
		, PFS_PETALOID_EMITTER(API_GET_PREF_NUMBER , emitGetNumberPref)
		, PFS_PETALOID_EMITTER(API_GET_PREF_STRING , emitGetStringPref)
	PFS_PETALOID_EMITTERS_END

	PFS_PETALOID_DETECTORS_BEGIN
	      PFS_PETALOID_DETECTOR(API_QUIT, petaloid::onQuit)
	    , PFS_PETALOID_DETECTOR(API_GUI_READY, petaloid::onGuiReady)
	PFS_PETALOID_DETECTORS_END

	static int main_loop (pfs::petaloid *);

public: // signals
	pfs::signal0<>                      emitQuit;
	pfs::signal1<const pfs::critical &> emitCritical;
	pfs::signal1<const pfs::string &>   emitInfo;
	pfs::signal1<const pfs::string &>   emitDebug;
	pfs::signal1<const pfs::string &>   emitWarn;
	pfs::signal1<const pfs::string &>   emitError;
	pfs::signal1<bool>                  emitGuiReady;
	pfs::signal2<const pfs::string &, bool *>        emitGetBooleanPref;
	pfs::signal2<const pfs::string &, integral_t *>  emitGetIntegerPref;
	pfs::signal2<const pfs::string &, real_t *>      emitGetNumberPref;
	pfs::signal2<const pfs::string &, pfs::string *> emitGetStringPref;

private: // slots
	void onQuit () {}
	void onGuiReady (bool);
	void onCritical (const pfs::critical & critical)  { if(_guiReady) emit qt.emitCritical(critical); }
	void onInfo  (const pfs::string & msg)            { if(_guiReady) emit qt.emitInfo  (pfs::platform::currentDateTime(), msg); }
	void onDebug (const pfs::string & msg)            { if(_guiReady) emit qt.emitDebug (pfs::platform::currentDateTime(), msg); }
	void onWarn  (const pfs::string & msg)            { if(_guiReady) emit qt.emitWarn  (pfs::platform::currentDateTime(), msg); }
	void onError (const pfs::string & msg)            { if(_guiReady) emit qt.emitError (pfs::platform::currentDateTime(), msg); }
};

} // ui


inline QtAdapter::QtAdapter()
{
	qRegisterMetaType<pfs::datetime>("pfs::string");
	qRegisterMetaType<pfs::critical>("pfs::critical");
	qRegisterMetaType<size_t>("size_t");
	connect(this, SIGNAL(emitCritical(const pfs::critical &)), SLOT(onCritical(const pfs::critical &)));
}

#endif /* __UI_QT_PETALOID_HPP__ */


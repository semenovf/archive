/**
 * @file petaloid.hpp
 *
 *  Created on: Jul 20, 2014
 *      Author: wladt
 */

#ifndef __HG_BOOKER_PETALOID_UI_QT_HPP__
#define __HG_BOOKER_PETALOID_UI_QT_HPP__

#include <cwt/petaloid.hpp>
#include <cwt/platform.hpp>
#include <cwt/critical.hpp>
#include "api.hpp"
#include <QObject>
#include <QString>
#include <QMetaType>

class MainWindow;

#define __PETALOID hg::booker::ui::petaloid::self()

class QtAdapter : public QObject
{
	Q_OBJECT

public:
	QtAdapter ();

public Q_SLOTS:
	void onCritical (const cwt::critical & critical);

Q_SIGNALS:
	void emitCritical   (const cwt::critical & critical);
	void emitInfo       (const pfs::datetime & dt, const pfs::string & msg);
	void emitDebug      (const pfs::datetime & dt, const pfs::string & msg);
	void emitWarn       (const pfs::datetime & dt, const pfs::string & msg);
	void emitError      (const pfs::datetime & dt, const pfs::string & msg);

//	friend class abtcws::ui::petaloid;
};


namespace hg { namespace booker { namespace ui {

class petaloid : public cwt::petaloid
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
//	petaloid () : cwt::petaloid("hg::booker::ui")
//	{

//		_self = this;
//	}

	virtual bool onStart ();
	virtual bool onFinish ();

	CWT_PETALOID_EMITTERS_BEGIN
	      CWT_PETALOID_EMITTER(API_QUIT      , emitQuit)
		, CWT_PETALOID_EMITTER(API_CRITICAL  , emitCritical)
		, CWT_PETALOID_EMITTER(API_INFO      , emitInfo)
		, CWT_PETALOID_EMITTER(API_DEBUG     , emitDebug)
		, CWT_PETALOID_EMITTER(API_WARN      , emitWarn)
		, CWT_PETALOID_EMITTER(API_ERROR     , emitError)
        , CWT_PETALOID_EMITTER(API_GET_PREF  , emitPref)
	    , CWT_PETALOID_EMITTER(API_GUI_READY , emitGuiReady)
	CWT_PETALOID_EMITTERS_END

	CWT_PETALOID_DETECTORS_BEGIN
	  	  CWT_PETALOID_DETECTOR(API_QUIT, petaloid::onQuit)
		, CWT_PETALOID_DETECTOR(API_GUI_READY, petaloid::onGuiReady)
	CWT_PETALOID_DETECTORS_END

	static int main_loop (cwt::petaloid *);

public: // signals
	cwt::signal0<>                      emitQuit;
	cwt::signal1<const cwt::critical &> emitCritical;
	cwt::signal1<const pfs::string &>   emitInfo;
	cwt::signal1<const pfs::string &>   emitDebug;
	cwt::signal1<const pfs::string &>   emitWarn;
	cwt::signal1<const pfs::string &>   emitError;
	cwt::signal2<const pfs::string &, pfs::unitype &> emitPref;
	cwt::signal1<bool>                  emitGuiReady;

private: // slots
	void onQuit () {}
	void onGuiReady (bool);
	void onCritical (const cwt::critical & critical)  { if(_guiReady) emit qt.emitCritical(critical); }
	void onInfo  (const pfs::string & msg)            { if(_guiReady) emit qt.emitInfo  (cwt::platform::currentDateTime(), msg); }
	void onDebug (const pfs::string & msg)            { if(_guiReady) emit qt.emitDebug (cwt::platform::currentDateTime(), msg); }
	void onWarn  (const pfs::string & msg)            { if(_guiReady) emit qt.emitWarn  (cwt::platform::currentDateTime(), msg); }
	void onError (const pfs::string & msg)            { if(_guiReady) emit qt.emitError (cwt::platform::currentDateTime(), msg); }
};

}}} // hg::booker::ui


inline QtAdapter::QtAdapter()
{
	qRegisterMetaType<pfs::datetime>("pfs::string");
	qRegisterMetaType<cwt::critical>("cwt::critical");
	qRegisterMetaType<size_t>("size_t");
	connect(this, SIGNAL(emitCritical(const cwt::critical &)), SLOT(onCritical(const cwt::critical &)));
}

#endif /* __HG_BOOKER_PETALOID_UI_QT_HPP__ */


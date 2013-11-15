/*
 * app.hpp
 *
 *  Created on: Aug 15, 2013
 *      Author: wladt
 */

#ifndef __CWT_APP_HPP__
#define __CWT_APP_HPP__

#include <cwt/sepaloid.hpp>
#include <cwt/option.hpp>
#include <cwt/vector.hpp>

CWT_NS_BEGIN

class MainProc
{
public:
	MainProc() {}
	virtual ~MainProc() {}
	virtual int operator () () { return 0; }
};

class DLL_API App
{
	CWT_DENY_COPY(App);
	CWT_IMPLEMENT_LOCKING(App);

public:
	App(const String & progname = String());
	~App() {}

	int exec() { return MainProc()(); }
	int exec(MainProc & mainProc) { return mainProc(); }
	int exec(Sepaloid & sepaloid) { return sepaloid.exec(); }
	const Settings & settings() const { return _settings; }
	Settings & settings() { return _settings; }

	static App * instance() { CWT_ASSERT(self); return self; }
	static App * app()      { CWT_ASSERT(self); return self; }

private:
	String   _program;
	Settings _settings;

private:
	static App * self;
};

CWT_NS_END

#endif /* __CWT_APP_HPP__ */

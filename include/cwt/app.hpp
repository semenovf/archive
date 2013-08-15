/*
 * app.hpp
 *
 *  Created on: Aug 15, 2013
 *      Author: wladt
 */

#ifndef __CWT_APP_HPP__
#define __CWT_APP_HPP__

#include <cwt/settings.hpp>
#include <cwt/options.hpp>

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
	App(int argc, char * argv[], size_t optc = 0, const Options * opts = nullptr);
	~App() {}

	int exec() { return MainProc()(); }
	int exec(MainProc & mainProc) { return mainProc(); }
	const Settings & settings() const { return m_settings; }
	Settings & settings() { return m_settings; }

	static App * instance() { CWT_ASSERT(self); return self; }
	static App * app()      { CWT_ASSERT(self); return self; }

private:
	Settings m_settings;

private:
	static App * self;
};

CWT_NS_END

#endif /* __CWT_APP_HPP__ */

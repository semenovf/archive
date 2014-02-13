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
#include <pfs/vector.hpp>

namespace cwt {

class DLL_API app
{
	PFS_DENY_COPY(app);
	PFS_IMPLEMENT_LOCKING(app);

public:
	class main_proc
	{
	public:
		main_proc() {}
		virtual ~main_proc() {}
		virtual int operator () () { return 0; }
	};

public:
	app (const pfs::string & progname = pfs::string());
	app (int argc, char * argv[]);
	~app () {}

	int exec () { return main_proc()(); }
	int exec (main_proc & mainProc) { return mainProc(); }
	int exec (cwt::sepaloid & sepaloid);

//	const cwt::settings & settings() const { return _settings; }
//	cwt::settings & settings() { return _settings; }

	static app * instance() { PFS_ASSERT(self); return self; }

private:
	pfs::string   _program;
//	cwt::settings _settings;

private:
	static app * self;
};

inline int app::exec (cwt::sepaloid & sepaloid)
{
	int r = EXIT_FAILURE;

    sepaloid.connectAll();
    if (sepaloid.start() && sepaloid.isGood()) {
   		r = sepaloid.exec();
    }
    sepaloid.logErrors();

	return r;
}


} // cwt

#endif /* __CWT_APP_HPP__ */

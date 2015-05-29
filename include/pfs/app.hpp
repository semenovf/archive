/*
 * app.hpp
 *
 *  Created on: Aug 15, 2013
 *      Author: wladt
 */

#ifndef __PFS_APP_HPP__
#define __PFS_APP_HPP__

#include <pfs/sepaloid.hpp>
#include <pfs/settings.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

class DLL_API app
{
	PFS_IMPLEMENT_LOCKING(app);

    string        _program;
    pfs::settings _settings;

private:
	static app * self;

public:
	class main_proc
	{
	public:
		main_proc() {}
		virtual ~main_proc() {}
		virtual int operator () () { return 0; }
	};

public:
	app (const string & progname = pfs::string());

	int exec () { return main_proc()(); }
	int exec (main_proc & mainProc) { return mainProc(); }
	int exec (sepaloid & sepaloid);

	pfs::settings & settings () { return _settings; }
	static app * instance() { PFS_ASSERT(self); return self; }
};

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_APP_HPP__ */

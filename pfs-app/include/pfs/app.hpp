/*
 * app.hpp
 *
 *  Created on: Aug 15, 2013
 *      Author: wladt
 */

#ifndef __PFS_APP_HPP__
#define __PFS_APP_HPP__

#include <pfs/dispatcher.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

class DLL_API app
{
	string _program;

private:
	static app * self;

public:
	struct main_proc
	{
		main_proc() {}
		virtual ~main_proc() {}
		virtual int operator () () { return 0; }
	};

public:
	app (const string & progname = string());

	int exec ()
	{
		return main_proc()();
	}

	int exec (main_proc & mainProc)
	{
		return mainProc();
	}

	int exec (dispatcher & d);

	static app * instance()
	{
		PFS_ASSERT(self); return self;
	}
};

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_APP_HPP__ */

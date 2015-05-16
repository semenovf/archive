/*
 * app.hpp
 *
 *  Created on: Aug 15, 2013
 *      Author: wladt
 */

#ifndef __PFS_APP_HPP__
#define __PFS_APP_HPP__

//#include <pfs/noncopyable.hpp>
//#include <pfs/option.hpp>
#include <pfs/sepaloid.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

class DLL_API app// : noncopyable
{
	PFS_IMPLEMENT_LOCKING(app);

private:
	string   _program;

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
	app (int argc
#if (defined(PFS_OS_WIN32) || defined(PFS_OS_WIN64)) && defined(PFS_UNICODE)
			, wchar_t * argv[]
#else
			, char * argv[]
#endif
	);
	~app () {}

	int exec () { return main_proc()(); }
	int exec (main_proc & mainProc) { return mainProc(); }
	int exec (sepaloid & sepaloid);

	static app * instance() { PFS_ASSERT(self); return self; }
};

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_APP_HPP__ */

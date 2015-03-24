/*
 * app.hpp
 *
 *  Created on: Aug 15, 2013
 *      Author: wladt
 */

#ifndef __PFS_APP_HPP__
#define __PFS_APP_HPP__

#include <pfs/vector.hpp>
#include <pfs/noncopyable.hpp>
#include <pfs/option.hpp>
#include <pfs/sepaloid.hpp>

namespace pfs {

class DLL_API app : noncopyable
{
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
	app (const string & progname = pfs::string());
	app (int argc, char * argv[]);
	~app () {}

	int exec () { return main_proc()(); }
	int exec (main_proc & mainProc) { return mainProc(); }
	int exec (sepaloid & sepaloid);

//	const cwt::settings & settings() const { return _settings; }
//	cwt::settings & settings() { return _settings; }

	static app * instance() { PFS_ASSERT(self); return self; }

private:
	string   _program;
//	cwt::settings _settings;

private:
	static app * self;
};

inline int app::exec (sepaloid & sepaloid)
{
	int r = EXIT_FAILURE;

    sepaloid.connectAll();
    if (sepaloid.start() && sepaloid.isGood()) {
   		r = sepaloid.exec();
    }
    sepaloid.logErrors();

	return r;
}


} // pfs

#endif /* __PFS_APP_HPP__ */

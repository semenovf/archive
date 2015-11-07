/*
 * dl.hpp
 *
 *  Created on: 06.07.2012
 *      Author: wladt
 */
#ifndef __PFS_DL_HPP__
#define __PFS_DL_HPP__

/* see http://en.wikipedia.org/wiki/Dynamic_loading */
#include <pfs/string.hpp>
#include <pfs/map.hpp>
#include <pfs/stringlist.hpp>
#include <pfs/errorable_ext.hpp>
#include <pfs/pluggable.hpp>
#include <pfs/noncopyable.hpp>

#ifdef PFS_CC_MSVC
#	include <windows.h>
#	include <windef.h>
#else
#	include <dlfcn.h>
#endif

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

class DLL_API dl : public errorable_ext, noncopyable
{
public:
#ifdef PFS_CC_MSC
	typedef HMODULE handle;
	typedef FARPROC symbol;
#else
	typedef void * handle;
	typedef void * symbol;
#endif

	typedef pfs::map<string, handle> plugin_map_type;

private:
	stringlist _searchPath;
	plugin_map_type _plugins;

protected:
	dl () {};

public:
	~dl ();

//	static dl & getPluginLoader () { return getDL(); }
//	static dl & getDL ();

	handle open (const string & path, string * realPath, bool global, bool resolve);

	handle open (const string & path, bool global, bool resolve)
	{
		return open(path, 0, global, resolve);
	}

	handle open (const string & path)
	{
		string unused; return open(path, 0, false, true);
	}

	symbol ptr   (handle h, const char * symname);
	void   close (handle h);

	pfs::pluggable * openPlugin (const string & name, const string & path);
	pfs::pluggable * openPlugin (const string & name);
	bool closePlugin (const string & name, pfs::pluggable * pluggable);

    string buildDlFileName (const string & basename);
	void   clearSearchPath () { _searchPath.clear(); }
	void   addSearchPath   (const string & dir) { _searchPath.append(dir); }

private:
	string searchFile (const string & filename);
};

} // pfs

EXTERN_C_BEGIN
int DLL_API dl_only_for_testing_purpose (void);
EXTERN_C_END

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /*__PFS_DL_HPP__ */

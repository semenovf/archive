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

class DLL_API dl : public errorable_ext
{
public:
#ifdef PFS_CC_MSC
	typedef HMODULE handle;
	typedef FARPROC symbol;
#else
	typedef void * handle;
	typedef void * symbol;
#endif

private:
	static stringlist _globalSearchPath;
	stringlist _searchPath;
	pfs::map<string, handle> _plugins;

public:
	dl () {};
	handle open (const string & path
			, bool global = false
			, bool resolve = true) {
		string unused; return open(path, unused, global, resolve);
	}
	handle open  (const string & path, string & realPath, bool global = false, bool resolve = true);
	symbol ptr   (handle h, const char * symname);
	void   close (handle h);

	bool   openPlugin  (const string & name, const string & path, pfs::pluggable * pluggable);
	bool   openPlugin  (const string & name, pfs::pluggable * pluggable);
	bool   closePlugin (const string & name, pfs::pluggable * pluggable);

    string buildDlFileName (const string & basename);
	void   clearSearchPath () { _searchPath.clear(); }
	void   addSearchPath   (const string & dir) { _searchPath.append(dir); }
	static void addGlobalSearchPath   (const string & dir) { _globalSearchPath.append(dir); }

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

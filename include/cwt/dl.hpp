/*
 * dl.hpp
 *
 *  Created on: 06.07.2012
 *      Author: wladt
 */


#ifndef __CWT_DL_HPP__
#define __CWT_DL_HPP__

/* see http://en.wikipedia.org/wiki/Dynamic_loading */
#include <pfs/string.hpp>
#include <pfs/map.hpp>
#include <pfs/vector.hpp>
#include <cwt/errorable.hpp>

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

namespace cwt {

class DLL_API dl : public errorable
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
	static pfs::vector<pfs::string> globalSearchPath;
	pfs::vector<pfs::string> searchPath;
	pfs::map<pfs::string, handle> plugins;

public:
	dl () {};
	handle open (const pfs::string & path
			, bool global = false
			, bool resolve = true) {
		pfs::string unused; return open(path, unused, global, resolve);
	}
	handle open            (const pfs::string & path, pfs::string & realPath, bool global = false, bool resolve = true);
	symbol ptr             (handle h, const char * symname);
	//static symbol symbol          (handle h, const char * symname) { return ptr(h, symname); }
	void   close           (handle h);
	pfs::string buildDlFileName (const pfs::string & basename);
	bool   find            (const pfs::string & path, pfs::string & realPath);
	void   clearSearchPath () { searchPath.clear(); }

	bool   pluginOpen  (const pfs::string & name, const pfs::string & path, void * pluggable);
	bool   pluginClose (const pfs::string & name, void * pluggable);

	void   addSearchPath   (const pfs::string & dir) { searchPath.append(dir); }
	static void addGlobalSearchPath   (const pfs::string & dir) { globalSearchPath.append(dir); }

private:
	pfs::string searchFile (const pfs::string & filename);
};

} // cwt

EXTERN_C_BEGIN
int DLL_API dl_only_for_testing_purpose (void);
EXTERN_C_END

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /*__CWT_DL_HPP__ */

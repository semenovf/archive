/*
 * dl.hpp
 *
 *  Created on: 06.07.2012
 *      Author: wladt
 */


#ifndef __CWT_DL_HPP__
#define __CWT_DL_HPP__

/* see http://en.wikipedia.org/wiki/Dynamic_loading */
#include <cwt/cwt.hpp>
#include <cwt/string.hpp>
#include <cwt/map.hpp>
#include <cwt/vector.hpp>

#ifdef CWT_CC_MSC
#	include <windows.h>
#	include <windef.h>
#else
#	include <dlfcn.h>
#endif

CWT_NS_BEGIN


class DLL_API Dl
{
public:
#ifdef CWT_CC_MSC
	typedef HMODULE Handle;
	typedef FARPROC Symbol;
#else
	typedef void * Handle;
	typedef void * Symbol;
#endif

public:
	static Handle open            (const String & path
			, bool global = true
			, bool resolve = true) {
		String unused; return open(path, unused, global, resolve);
	}
	static Handle open            (const String & path, String & realPath, bool global = true, bool resolve = true);
	static Symbol ptr             (Handle h, const char * symname);
	static Symbol symbol          (Handle h, const char * symname) { return ptr(h, symname); }
	static void   close           (Handle h);
	static String buildDlFileName (const String & basename);
	static bool   find            (const String & path, String & realPath);
	static void   addSearchPath   (const String & dir) { searchPath.append(dir); }
	static void   clearSearchPath () { searchPath.clear(); }

	static bool   pluginOpen  (const String & name, const String & path, void * pluggable);
	static bool   pluginClose (const String & name, void * pluggable);

private:
	static Vector<String> searchPath;
	static Map<String, Handle> plugins;
};

EXTERN_C_BEGIN
int DLL_API dl_only_for_testing_purpose(void);
EXTERN_C_END

CWT_NS_END

#endif /*__CWT_DL_HPP__ */

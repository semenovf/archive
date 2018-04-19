#include "../../include/cwt/dl.hpp"
#include "../../include/cwt/logger.hpp"
#include "../../include/cwt/mt.hpp"

/* TODO need error reporting */

CWT_NS_BEGIN

extern String strerror_win32(uint_t errn);


inline bool __file_exists (const String & path)
{
	struct _stat st;
	return ( _wstat(path.utf16(), &st ) == 0 );
}
// see cwt::fs
inline bool __is_absolute_path (const String & path)
{
	static String __disks("abcdefghigklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");

	if ( path.isEmpty()) {
		return false;
	}

	if (path.startsWith(String("\\"))) {
		return true;
	}

	if (path.length() > 2
			&& __disks.find(String(1, path[0])) == __disks.cbegin()
			&& path[1] == UChar(':')) {
		return true;
	}

	return false;
}


Dl::Handle Dl::open (const String & path, String & realPath, bool global, bool resolve)
{
	static mt_policy_t mutex;
	AutoLock<> locker(&mutex);

	Dl::Handle h = NULL;

	DWORD dwFlags = 0;
	CWT_UNUSED(global);

	realPath.clear();

	if (!__file_exists(path)) {
		if (!__is_absolute_path(path)) {
			Vector<String>::const_iterator it = searchPath.cbegin();
			Vector<String>::const_iterator itEnd = searchPath.cend();
			while (it != itEnd) {
				realPath = *it + String("/") + path;
				if (!__file_exists(realPath))
					break;
				++it;
			}
		}
	}

	if (realPath.isEmpty())
		realPath = path;

	if (!resolve)
		dwFlags |= DONT_RESOLVE_DLL_REFERENCES;

	h = LoadLibraryEx(realPath.toUtf16().data(), NULL, dwFlags);

	if( !h ) {
		Logger::error(_Fr("%s: failed to open dynamic library: %s")
				% path % strerror_win32(GetLastError()));
	} else {
	   /*use the result in a call to GetProcAddress*/
	}

	return h;
}

Dl::Symbol Dl::ptr (Dl::Handle h, const char *symname)
{
	Dl::Symbol p = NULL;

	CWT_ASSERT(symname);

	p = GetProcAddress(h, symname);
	if (!p) {
		Logger::error(_Fr("%s: symbol not found: %s") % symname % strerror_win32(GetLastError()));
	}
	return p;
}

void Dl::close (Dl::Handle h)
{
	static mt_policy_t mutex;
	AutoLock<> locker(&mutex);

	if( h != (Dl::Handle)0) {
		FreeLibrary(h);
		h = (Dl::Handle)0;
	}
}

/**
 * @brief Builds dynamic library full name according to platform.
 *
 * @param name base name of dynamic lubrary
 * @param libname full library name to store
 */
String Dl::buildDlFileName (const String &basename)
{
	String libname;
	libname.append(basename);
	libname.append(String(".dll"));
	return libname;
}

CWT_NS_END

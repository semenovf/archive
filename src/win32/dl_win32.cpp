#include <cwt/dl.hpp>

/* TODO need error reporting */

CWT_NS_BEGIN

extern String strerror_win32(uint errn);

Dl::Handle Dl::open (const String &path, bool global, bool resolve)
{
	Dl::Handle h = NULL;

	DWORD dwFlags = 0;
	CWT_UNUSED(global);

	if (!resolve)
		dwFlags |= DONT_RESOLVE_DLL_REFERENCES;

	h = LoadLibraryEx(path.utf16(), NULL, dwFlags);

	if( !h ) {
		Logger::error( _Tr("%ls: failed to open dynamic library: %s"), path, strerror_win32(GetLastError()));
	} else {
	   /*use the result in a call to GetProcAddress*/
	}

	return h;
}

Dl::Ptr Dl::ptr (Dl::Handle h, const char *symname)
{
	Dl::Ptr p = NULL;

	CWT_ASSERT(symname);

	p = GetProcAddress(h, symname);
	if (!p) {
		Logger::error(_Tr("%s: symbol not found: %s"), symname, strerror_win32(GetLastError()));
	}
	return p;
}

void Dl::close (Dl::Handle h)
{
	if( h != (DlHandle)0) {
		FreeLibrary(h);
		h = (DlHandle)0;
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
	libname.append(_U(".dll"));
	return libname;
}

CWT_NS_END

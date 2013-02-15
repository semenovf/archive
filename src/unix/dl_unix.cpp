#include <cwt/dl.hpp>
#include <cwt/logger.hpp>

/* TODO need error reporting */

CWT_NS_BEGIN

Dl::Handle Dl::open (const String &path, bool global, bool resolve)
{
	Dl::Handle h = NULL;
	ByteArray utf8path(path.toUtf8());

	dlerror(); /* clear error */
	h = dlopen( utf8path.data(), (global ? RTLD_GLOBAL : RTLD_LOCAL) | ( resolve ? RTLD_NOW : RTLD_LAZY ) );
	if( !h ) {
		Logger::error( _Tr("%ls: failed to open dynamic library: %s"), path.utf16(), dlerror());
	}
	return h;
}

Dl::Ptr Dl::ptr (Dl::Handle h, const char *symname)
{
	Dl::Ptr p = NULL;

	dlerror(); /*clear error*/
	p = dlsym(h, symname);
	if (!p) {
		Logger::error(_Tr("%s: symbol not found: %s"), symname, dlerror());
	}
	return p;
}

void Dl::close (Dl::Handle h)
{
	if( h != (Dl::Handle)0) {
		dlerror(); /*clear error*/
		dlclose(h);
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
	libname.append(_U("lib"));
	libname.append(basename);
	libname.append(_U(".so"));
	return libname;
}

CWT_NS_END

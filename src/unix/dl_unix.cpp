#include <cwt/dl.hpp>

/* TODO need error reporting */

CWT_NS_BEGIN

Dl::Handle Dl::open (const String &path, bool global, bool resolve)
{
	Dl::Handle h = NULL;
	ByteArray utf8path(path.toUtf8());

	dlerror(); /* clear error */
	h = dlopen( utf8path.data(), (global ? RTLD_GLOBAL : RTLD_LOCAL) | ( resolve ? RTLD_NOW : RTLD_LAZY ) );
/*
	if( !h ) {
		cwt_logger_ns()->error( _Tr("%s: failed to open dynamic library: %s"), path, dlerror() );
	}
*/
	return h;
}

Dl::Ptr Dl::ptr (Dl::Handle h, const String &symname)
{
	Dl::Ptr p = NULL;
	ByteArray utf8symname(symname.toUtf8());

	dlerror(); /*clear error*/
	p = dlsym(h, utf8symname.data());
/*
	if( !sym ) {
		cwt_logger_ns()->error(_Tr("%s: symbol not found: error code=%lu"), sym_name, dlerror());
	}
*/
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

#include "../../include/cwt/dl.hpp"
#include "../../include/cwt/safeformat.hpp"
#include "../../include/cwt/logger.hpp"
#include <sys/stat.h>

/* TODO need error reporting */

CWT_NS_BEGIN

inline bool __file_exists (const String & path)
{
	struct stat st;
	return ( stat(path.c_str(), &st ) == 0 );
}

inline bool __is_absolute_path (const String & path)
{
	return path.startsWith(String("/"));
}

Dl::Handle Dl::open (const String & path, String & realPath, bool global, bool resolve)
{
	Dl::Handle h = NULL;

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

	dlerror(); /* clear error */
	h = dlopen( realPath.c_str(), (global ? RTLD_GLOBAL : RTLD_LOCAL) | ( resolve ? RTLD_NOW : RTLD_LAZY ) );
	if (!h) {
		Logger::error(_Fr("%s (%s): failed to open dynamic library: %s") % path % realPath % dlerror());
	}
	return h;
}

bool Dl::opened (const String & path)
{
	dlerror(); /* clear error */
	return dlopen( path.c_str(), RTLD_LAZY | RTLD_NOLOAD) != NULL ? true : false;
}

Dl::Symbol Dl::ptr (Dl::Handle h, const char *symname)
{
	Dl::Symbol p = NULL;

	dlerror(); /*clear error*/
	p = dlsym(h, symname);
	if (!p) {
		Logger::error(_Fr("%s: symbol not found: %s") % symname % dlerror());
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
	libname.append(String("lib"));
	libname.append(basename);
	libname.append(String(".so"));
	return libname;
}

CWT_NS_END

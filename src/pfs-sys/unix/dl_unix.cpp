#include "pfs/dl.hpp"
#include <pfs/mt.hpp>
#include <sys/stat.h>

namespace pfs {

dl::handle dl::open (const pfs::string & path, pfs::string & realPath, bool global, bool resolve)
{
	static pfs::mutex __mutex;
	pfs::auto_lock<> locker(& __mutex);

	dl::handle h = nullptr;

	realPath = searchFile(path);

	if (!realPath.isEmpty()) {
		dlerror(); /* clear error */
		h = dlopen(realPath.c_str(), (global ? RTLD_GLOBAL : RTLD_LOCAL) | ( resolve ? RTLD_NOW : RTLD_LAZY ));
		if (!h) {
			pfs::string errstr;
			errstr << path << " (" << realPath << "): "
					<< _Tr("failed to open dynamic library")
					<< ": "
					<< pfs::string::fromUtf8(dlerror());
			addError(errstr);
		}
	}
	return h;
}

/*
bool dl::opened (const pfs::string & path)
{
	dlerror(); // clear error
	return dlopen( path.c_str(), RTLD_LAZY | RTLD_NOLOAD) != NULL ? true : false;
}
*/

dl::symbol dl::ptr (dl::handle h, const char * symname)
{
	dl::symbol p = NULL;

	dlerror(); /*clear error*/
	p = dlsym(h, symname);
	if (!p) {
		pfs::string errstr;
		errstr << symname << ": "
				<< _Tr("symbol not found")
				<< ": "
				<< pfs::string::fromUtf8(dlerror());
		addError(errstr);
	}
	return p;
}

void dl::close (dl::handle h)
{
	static pfs::mutex __mutex;
	pfs::auto_lock<> locker(& __mutex);

	if( h != (dl::handle)0) {
		dlerror(); /*clear error*/
		dlclose(h);
		h = (dl::handle)0;
	}
}

/**
 * @brief Builds dynamic library full name according to platform.
 *
 * @param name base name of dynamic lubrary
 * @param libname full library name to store
 */
pfs::string dl::buildDlFileName (const pfs::string & basename)
{
	pfs::string libname;
	libname.append(pfs::string("lib"));
	libname.append(basename);
	libname.append(pfs::string(".so"));
	return libname;
}

} //pfs

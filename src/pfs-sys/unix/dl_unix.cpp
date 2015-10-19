//#include <pfs/mutex.hpp>
#include <sys/stat.h>
#include "pfs/dl.hpp"

namespace pfs {

/**
 * @brief Open dynamic library (shared object)
 *
 * @param path path to dynamic library file (relative or absolute)
 * @param realPathPtr
 * @param global
 * @param resolve
 * @return
 */
dl::handle dl::open (const pfs::string & path, pfs::string * realPathPtr, bool global, bool resolve)
{
//	static pfs::mutex mtx;
//	pfs::lock_guard<pfs::mutex> locker(mtx);

	dl::handle h = nullptr;

	if (realPathPtr)
		realPathPtr->clear();

	pfs::string realPath = searchFile(path);

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

		if (realPathPtr)
			*realPathPtr = realPath;
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
//	static pfs::mutex mtx;
//	pfs::lock_guard<pfs::mutex> locker(mtx);

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

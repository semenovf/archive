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
dl::handle dl::open (const fs::path & path, fs::path * result_path_ptr, bool global, bool resolve)
{
//	static pfs::mutex mtx;
//	pfs::lock_guard<pfs::mutex> locker(mtx);

	dl::handle h = 0;

	if (result_path_ptr)
		realPathPtr->clear();

	fs::path real_path = search_file(path);

	if (!real_path.empty()) {
		dlerror(); /* clear error */
		h = dlopen(real_path.native().c_str(), (global ? RTLD_GLOBAL : RTLD_LOCAL) | ( resolve ? RTLD_NOW : RTLD_LAZY ));

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

dl::symbol dl::resolve (const char * symbol_name, error_code * ex)
{
	dlerror(); /*clear error*/
	dl::symbol r = dlsym(_handle, symbol_name);

	if (!r) {
		if (ex)
			*ex = DlSymbolNotFoundError;
		return handle(0);
	}

	return r;
}

void dl::close ()
{
//	static pfs::mutex mtx;
//	pfs::lock_guard<pfs::mutex> locker(mtx);

	if (_handle) {
		dlerror(); /*clear error*/
		dlclose(_handle);
		_handle = (dl::handle)0;
	}
}

/**
 * @brief Builds dynamic library full name according to platform.
 *
 * @param name base name of dynamic lubrary
 * @param libname full library name to store
 */
fs::path dl::build_filename (const string & name)
{
	return fs::path(string("lib") + name + string(".so"));
}

} //pfs

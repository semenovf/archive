//#include <pfs/mutex.hpp>
#include <sys/stat.h>
#include "pfs/dl.hpp"

namespace pfs {

bool dl::open (const fs::path & p, const fs::pathlist & searchdirs
		, error_code * ex
		, string * extended_errstr)
{
//	static pfs::mutex mtx;
//	pfs::lock_guard<pfs::mutex> locker(mtx);

	if (ex)
		*ex = 0;

	if (extended_errstr)
		extended_errstr->clear();

	dl::handle h = 0;

	_path = p;

	if (_path.empty()) {
		if (ex)
			*ex = InvalidArgument;
		return false;
	}

	if (!fs::exists(_path)) {
		_path = fs::search_file(_path, searchdirs, ex);

		if (_path.empty())
			return false;
	}

	// clear error
	//
	dlerror();

	bool global = false;
	bool resolve = true;

	h = dlopen(_path.native().c_str()
			, (global ? RTLD_GLOBAL : RTLD_LOCAL)
				| ( resolve ? RTLD_NOW : RTLD_LAZY));

	if (!h) {
		if (ex)
			*ex = DlOpenError;

		if (extended_errstr) {
			extended_errstr->append(to_string(_path));
			extended_errstr->append(_u8(": failed to open dynamic library: "));
			extended_errstr->append(_u8(dlerror()));
		}

		_path = fs::path();
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

dl::symbol dl::resolve (const char * symbol_name
		, error_code * ex
		, string * extended_errstr)
{
	if (ex)
		*ex = 0;

	if (extended_errstr)
		extended_errstr->clear();

	// clear error
	//
	dlerror();

	dl::symbol r = dlsym(_handle, symbol_name);
	char * errstr = dlerror();

	if (errstr) {
		if (ex)
			*ex = DlSymbolNotFoundError;

		if (extended_errstr) {
			extended_errstr->append(to_string(_path));
			extended_errstr->append(_u8(": failed to resolve symbol `"));
			extended_errstr->append(_u8(symbol_name));
			extended_errstr->append(_u8("': "));
			extended_errstr->append(_u8(dlerror()));
		}

		return symbol(0);
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
	string s("lib");
	s.append(name);
	s.append(".so");
	return fs::path(s);
}

} //pfs

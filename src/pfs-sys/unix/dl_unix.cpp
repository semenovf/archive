//#include <pfs/mutex.hpp>
#include <pfs/dl.hpp>
#include <sys/stat.h>

namespace pfs {

dynamic_library::shared_data::~shared_data ()
{
	if (_handle) {
		dlerror(); /*clear error*/
		dlclose(_handle);
		_handle = (dynamic_library::handle)0;
	}
}


bool dynamic_library::open (const fs::path & p, const fs::pathlist & searchdirs
		, error_code * ex
		, string * extended_errstr)
{
//	static pfs::mutex mtx;
//	pfs::lock_guard<pfs::mutex> locker(mtx);

	if (ex)
		*ex = 0;

	if (extended_errstr)
		extended_errstr->clear();

	dynamic_library::handle h = 0;

	_d->_path = p;

	if (_d->_path.empty()) {
		if (ex)
			*ex = InvalidArgument;
		return false;
	}

	if (!fs::exists(_d->_path)) {
		_d->_path = fs::search_file(_d->_path, searchdirs, ex);

		if (_d->_path.empty()) {
			if (ex) *ex = ENOENT;
			return false;
		}
	}

	// clear error
	//
	dlerror();

	bool global = false;
	bool resolve = true;

	h = dlopen(_d->_path.native().c_str()
			, (global ? RTLD_GLOBAL : RTLD_LOCAL)
				| ( resolve ? RTLD_NOW : RTLD_LAZY));

	if (!h) {
		if (ex)
			*ex = DlOpenError;

		if (extended_errstr) {
			extended_errstr->append(to_string(_d->_path));
			extended_errstr->append(_u8(": failed to open dynamic library: "));
			extended_errstr->append(_u8(dlerror()));
		}

		_d->_path = fs::path();
	}

	_d->_handle = h;

	return h != 0;
}

/*
bool dl::opened (const pfs::string & path)
{
	dlerror(); // clear error
	return dlopen( path.c_str(), RTLD_LAZY | RTLD_NOLOAD) != NULL ? true : false;
}
*/

dynamic_library::symbol dynamic_library::resolve (const char * symbol_name
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

	dynamic_library::symbol r = dlsym(_d->_handle, symbol_name);
	char * errstr = dlerror();

	if (errstr) {
		if (ex)
			*ex = DlSymbolNotFoundError;

		if (extended_errstr) {
			extended_errstr->append(to_string(_d->_path));
			extended_errstr->append(_u8(": Failed to resolve symbol `"));
			extended_errstr->append(_u8(symbol_name));
			extended_errstr->append(_u8("': "));
			extended_errstr->append(_u8(errstr));
		}

		return symbol(0);
	}

	return r;
}

/**
 * @brief Builds dynamic library full name according to platform.
 *
 * @param name base name of dynamic lubrary
 * @param libname full library name to store
 */
fs::path dynamic_library::build_filename (const string & name)
{
	string s("lib");
	s.append(name);
	s.append(".so");
	return fs::path(s);
}

} //pfs

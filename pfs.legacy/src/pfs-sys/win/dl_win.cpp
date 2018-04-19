#include <pfs/dl.hpp>
#include "pfs/fs.hpp"
#include <pfs/mt.hpp>

namespace pfs {

dynamic_library::handle dynamic_library::open (const pfs::string & path, pfs::string & realPath, bool global, bool resolve)
{
	static pfs::mutex __mutex;
	pfs::auto_lock<> locker(&__mutex);

	dynamic_library::handle h = 0;

	DWORD dwFlags = 0;
	PFS_UNUSED(global);

	pfs::fs fs;
	realPath = searchFile(path);

	if (!realPath.isEmpty()) {
		if (!resolve)
			dwFlags |= DONT_RESOLVE_DLL_REFERENCES;

		h = LoadLibraryEx(realPath.toUtf16().data(), NULL, dwFlags);

		if( !h ) {
			pfs::string errcaption;
			errcaption << path << ": "
					<< _Tr("failed to open dynamic library");
			addSystemError(int(GetLastError()), errcaption);
		} else {
		   /*use the result in a call to GetProcAddress*/
		}
	}

	return h;
}

dynamic_library::symbol dynamic_library::ptr (dynamic_library::handle h, const char *symname)
{
	dynamic_library::symbol p = 0;

	PFS_ASSERT(symname);

	p = GetProcAddress(h, symname);
	if (!p) {
		pfs::string errcaption;
		errcaption << symname << ": "
				<< _Tr("symbol not found");
		addSystemError(int(GetLastError()), errcaption);
	}
	return p;
}

void dynamic_library::close (dynamic_library::handle h)
{
	static pfs::mutex __mutex;
	pfs::auto_lock<> locker(&__mutex);

	if( h != (dynamic_library::handle)0) {
		FreeLibrary(h);
		h = (dynamic_library::handle)0;
	}
}

/**
 * @brief Builds dynamic library full name according to platform.
 *
 * @param name base name of dynamic lubrary
 * @param libname full library name to store
 */
pfs::string dynamic_library::buildDlFileName (const pfs::string & basename)
{
	pfs::string libname;
	libname.append(basename);
	libname.append(pfs::string(".dll"));
	return libname;
}

} // pfs

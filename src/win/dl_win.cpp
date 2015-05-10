#include "pfs/dl.hpp"
#include "pfs/fs.hpp"
#include <pfs/mt.hpp>

namespace pfs {

dl::handle dl::open (const pfs::string & path, pfs::string & realPath, bool global, bool resolve)
{
	static pfs::mutex __mutex;
	pfs::auto_lock<> locker(&__mutex);

	dl::handle h = nullptr;

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

dl::symbol dl::ptr (dl::handle h, const char *symname)
{
	dl::symbol p = nullptr;

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

void dl::close (dl::handle h)
{
	static pfs::mutex __mutex;
	pfs::auto_lock<> locker(&__mutex);

	if( h != (dl::handle)0) {
		FreeLibrary(h);
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
	libname.append(basename);
	libname.append(pfs::string(".dll"));
	return libname;
}

} // pfs

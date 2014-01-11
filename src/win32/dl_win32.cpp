#include "../../include/cwt/dl.hpp"
#include "../../include/cwt/fs.hpp"
#include <pfs/mt.hpp>

namespace cwt {

inline bool __is_absolute_path (const pfs::string & path)
{
	static pfs::string __disks("abcdefghigklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");

	if ( path.isEmpty()) {
		return false;
	}

	if (path.startsWith(pfs::string("\\"))) {
		return true;
	}

	if (path.length() > 2
			&& __disks.find(pfs::string(1, path[0])) == __disks.cbegin()
			&& path[1] == pfs::ucchar(':')) {
		return true;
	}

	return false;
}


dl::handle dl::open (const pfs::string & path, pfs::string & realPath, bool global, bool resolve)
{
	static pfs::mutex __mutex;
	pfs::auto_lock<> locker(&__mutex);

	dl::handle h = NULL;

	DWORD dwFlags = 0;
	PFS_UNUSED(global);

	realPath.clear();
	cwt::fs fs;

	if (!fs.exists(path)) {
		if (!__is_absolute_path(path)) {
			pfs::vector<pfs::string>::const_iterator it = searchPath.cbegin();
			pfs::vector<pfs::string>::const_iterator itEnd = searchPath.cend();
			while (it != itEnd) {
				realPath = *it + pfs::string("/") + path;
				if (!fs.exists(realPath))
					break;
				++it;
			}
		}
	}

	if (realPath.isEmpty())
		realPath = path;

	if (!resolve)
		dwFlags |= DONT_RESOLVE_DLL_REFERENCES;

	h = LoadLibraryEx(realPath.toUtf16().data(), NULL, dwFlags);

	if( !h ) {
		pfs::string errcaption;
		errcaption << path << ": "
				<< _Tr("failed to open dynamic library");
		addSystemError(int_t(GetLastError()), errcaption);
	} else {
	   /*use the result in a call to GetProcAddress*/
	}

	return h;
}

dl::symbol dl::ptr (dl::handle h, const char *symname)
{
	dl::symbol p = NULL;

	PFS_ASSERT(symname);

	p = GetProcAddress(h, symname);
	if (!p) {
		pfs::string errcaption;
		errcaption << symname << ": "
				<< _Tr("symbol not found");
		addSystemError(int_t(GetLastError()), errcaption);
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

} // cwt

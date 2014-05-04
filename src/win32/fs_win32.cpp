/*
 * filesystem_win32.cpp
 *
 *  Created on: Sep 14, 2011
 *      Author: wladt
 */

#include <cwt/fs.hpp>
#include <cwt/safeformat.hpp>

namespace cwt {

static pfs::string __separator("\\");

pfs::ucchar fs::separator () const
{
	return __separator[0];
}


bool fs::isAbsolute(const pfs::string &path)
{
	static pfs::string __disks("abcdefghigklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");

	if ( path.isEmpty()) {
		return false;
	}

	if (path.startsWith(__separator)) {
		return true;
	}

	if (path.length() > 2
			&& __disks.find(pfs::string(1, path[0])) == __disks.cbegin()
			&& path[1] == pfs::ucchar(':')) {
		return true;
	}

	return false;
}

bool fs::exists(const pfs::string & path)
{
	PFS_CHECK_SIZEOF_TYPE(wchar_t,2);
	struct _stat st;
	return _wstat(path.toUtf16().data(), & st ) == 0;
}


bool fs::rename(const pfs::string & from, const pfs::string & to)
{
	if (!MoveFile(from.toUtf16().data(), to.toUtf16().data())) {
		pfs::string errstr;
		errstr << _Tr("Unable to rename file")
				<< _Tr(" from ") << from
				<< _Tr(" to ") << to;
		addSystemError(GetLastError(), errstr);
		return false;
	}
	return true;
}

bool fs::remove(const pfs::string & path)
{
	if (!DeleteFile(path.toUtf16().data())) {
		pfs::string errstr;
		errstr << _Tr("Unable to unlink (delete) file ") << path;
		addSystemError(GetLastError(), errstr);
		return false;
	}
	return true;
}

size_t fs::size (const pfs::string & path) const
{
	PFS_CHECK_SIZEOF_TYPE(wchar_t,2);
	struct _stat st;
	return _wstat(path.toUtf16().data(), & st ) == 0
			? size_t(st.st_size)
			: size_t(0);
}

bool fs::simpleBackup(const pfs::string &orig)
{
	pfs::string to(orig);
	to.append(pfs::string(".bak"));
	return fs::rename(orig, to);
}

// TODO should be tested
pfs::string fs::tempDirectory ()
{
	TCHAR path[256];
	TCHAR *ppath = & path[0];
	DWORD sz = ::GetTempPath(256, ppath);
	pfs::string r;

	if (sz > 256) {
		ppath = (TCHAR*)malloc(sz * sizeof(TCHAR));
		sz = ::GetTempPath(sz * sizeof(TCHAR), ppath);
	}

	if (sz == 0) {
		addSystemError(GetLastError(), _u8(_Tr("Unable to get temporary directory")));
	} else {
		// String returned by GetTempPath ends with a backslash, for example, "C:\TEMP\",
		// so need to remove it.
		if (ppath[sz - 1] == pfs::ucchar('\\')) {
			ppath[sz - 1] = pfs::ucchar('\x0');
#ifdef _UNICODE
			r = pfs::string::fromUtf16(ppath, sz);
#else
			r = pfs::string::fromLatin1(ppath);
#endif
		} else {
			addError(_u8(_Tr("Failed to get temporary directory")));
		}
	}

	if (ppath && ppath != & path[0]) {
		free(ppath);
	}

	return r;
}


} // cwt

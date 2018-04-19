/*
 * filesystem_win32.cpp
 *
 *  Created on: Sep 14, 2011
 *      Author: wladt
 */

#if __OBSOLETE__ // XXX OBSOLETE
#include "pfs/fs.hpp"

namespace pfs {

static string __separator("\\");

fs::fs ()
	: _separator(__separator[0])
{}

bool fs::isAbsolute (const string & path) const
{
	static string __disks("abcdefghigklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");

	if ( path.isEmpty()) {
		return false;
	}

	if (path.startsWith(__separator)) {
		return true;
	}

	if (path.length() > 2
			&& __disks.find(string(1, path[0])) == __disks.cbegin()
			&& path[1] == pfs::ucchar(':')) {
		return true;
	}

	return false;
}

bool fs::exists (const string & path) const
{
	PFS_ASSERT_RANGE(sizeof(wchar_t) == 2);
	struct _stat st;
	return _wstat(path.toUtf16().data(), & st ) == 0;
}


bool fs::rename(const string & from, const string & to)
{
	if (!MoveFile(from.toUtf16().data(), to.toUtf16().data())) {
		string errstr;
		errstr << _Tr("Unable to rename file")
				<< _Tr(" from ") << from
				<< _Tr(" to ") << to;
		addSystemError(GetLastError(), errstr);
		return false;
	}
	return true;
}

bool fs::remove(const string & path)
{
	if (!DeleteFile(path.toUtf16().data())) {
		string errstr;
		errstr << _Tr("Unable to unlink (delete) file ") << path;
		addSystemError(GetLastError(), errstr);
		return false;
	}
	return true;
}

size_t fs::size (const string & path) const
{
	PFS_ASSERT_RANGE(sizeof(wchar_t) == 2);
	struct _stat st;
	return _wstat(path.toUtf16().data(), & st ) == 0
			? size_t(st.st_size)
			: size_t(0);
}

bool fs::simpleBackup(const string &orig)
{
	string to(orig);
	to.append(string(".bak"));
	return fs::rename(orig, to);
}

string fs::tempDirectory ()
{
	TCHAR path[256];
	TCHAR * ppath = & path[0];
	DWORD sz = ::GetTempPath(256, ppath);
	string r;

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
#ifdef PFS_UNICODE
			r = string::fromUtf16(ppath, sz);
#else
			r = string::fromLatin1(ppath); // TODO Replace with fromLocal8Bit()
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

} // pfs
#endif

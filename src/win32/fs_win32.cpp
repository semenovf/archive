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

pfs::ucchar fs::separator()
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


bool fs::simpleBackup(const pfs::string &orig)
{
	pfs::string to(orig);
	to.append(pfs::string(".bak"));
	return FileSystem::rename(orig, to);
}

} // cwt

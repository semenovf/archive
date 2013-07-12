/*
 * filesystem_win32.cpp
 *
 *  Created on: Sep 14, 2011
 *      Author: wladt
 */

#include <cwt/filesystem.hpp>
#include <cwt/logger.hpp>

CWT_NS_BEGIN

extern String strerror_win32(uint_t errn);
static String __separator(_U("\\"));

Char FileSystem::separator()
{
	return __separator[0];
}


bool FileSystem::isAbsolute(const String &path)
{
	static String __disks(_U("abcdefghigklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"));

	if ( path.isEmpty()) {
		return false;
	}

	if (path.startsWith(__separator)) {
		return true;
	}

	if (path.length() > 2
			&& __disks.indexOf(path[0]) == 0
			&& path.indexOf(_U(":")) == 1) {
		return true;
	}

	return false;
}

bool FileSystem::exists(const String &path)
{
	struct _stat st;
	return ( _wstat(path.utf16(), &st ) == 0 );
/*

	struct stat st;
	return ( stat(path.toUtf8().data(), &st ) == 0 );
*/
}


bool FileSystem::rename(const String &from, const String &to)
{
	if (!MoveFile(from.utf16(), to.utf16())) {
		Logger::error(_Tr("unable to rename file from %ls to %ls: %s")
			, from, to, strerror_win32(GetLastError()));
		return false;
	}
	return true;
}

bool FileSystem::remove(const String &path)
{
	if (!DeleteFile(path.utf16())) {
		Logger::error(_Tr("unable to unlink (delete) file %ls: %s")
			, path, strerror_win32(GetLastError()));
		return false;
	}
	return true;
}


bool FileSystem::simpleBackup(const String &orig)
{
	String to(orig);
	to.append(_U(".bak"));
	return FileSystem::rename(orig, to);
}

CWT_NS_END



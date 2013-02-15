/*
 * filesystem.cpp
 *
 *  Created on: Feb 15, 2013
 *      Author: wladt
 *
 */

#include <cwt/filesystem.hpp>
#include <cwt/logger.hpp>
#include <sys/stat.h>
#include <cstring>
#include <cerrno>
#include <unistd.h>

CWT_NS_BEGIN

static String __separator(_U("/"));

Char FileSystem::separator()
{
	return __separator[0];
}


bool FileSystem::isAbsolute(const String &path)
{
	if( path.isEmpty() )
		return false;

	return path.startsWith(__separator);
}

bool FileSystem::exists(const String &path)
{
	struct stat st;
	return ( stat(path.toUtf8().data(), &st ) == 0 );
}


bool FileSystem::rename(const String &from, const String &to)
{
	if (::rename(from.toUtf8().data(), to.toUtf8().data()) != 0) {
		Logger::error(_Tr("unable to rename file from %ls to %ls: %s")
			, from.utf16(), to.utf16(), strerror(errno));
		return false;
	}
	return true;
}

bool FileSystem::remove(const String &path)
{
	if( ::unlink(path.toUtf8().data()) != 0) {
		Logger::error(_Tr("unable to unlink (delete) file %ls: %s")
			, path.utf16(), strerror(errno));
		return false;
	}
	return true;
}


bool FileSystem::simpleBackup(const String &orig)
{
	String to(orig);
	to.append(_U("~"));
	return FileSystem::rename(orig, to);
}

CWT_NS_END

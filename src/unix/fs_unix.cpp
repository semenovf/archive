/*
 * filesystem.cpp
 *
 *  Created on: Feb 15, 2013
 *      Author: wladt
 *
 */

#include <cwt/fs.hpp>
#include <cwt/safeformat.hpp>
#include <sys/stat.h>
#include <cstring>
#include <cerrno>
#include <unistd.h>

namespace cwt {

pfs::ucchar fs::separator()
{
	return pfs::ucchar('/');
}


bool fs::isAbsolute(const pfs::string & path)
{
	if( path.isEmpty() )
		return false;

	return path.startsWith(pfs::string(1, separator()));
}

bool fs::exists (const pfs::string & path)
{
	struct stat st;
	return ( stat(path.c_str(), &st ) == 0 );
}


bool fs::rename (const pfs::string & from, const pfs::string & to)
{
	if (::rename(from.c_str(), to.c_str()) != 0) {
		pfs::string errstr;
		errstr << _Tr("Unable to rename file")
				<< _Tr(" from ") << from
				<< _Tr(" to ") << to;
		addSystemError(errno, errstr);
		return false;
	}
	return true;
}

bool fs::remove (const pfs::string & path)
{
	if( ::unlink(path.c_str()) != 0) {
		pfs::string errstr;
		errstr << _Tr("Unable to unlink (delete) file ") << path;
		addSystemError(errno, errstr);
		return false;
	}
	return true;
}


bool fs::simpleBackup (const pfs::string & orig)
{
	pfs::string to(orig);
	to.append(pfs::string(1, '~'));
	return fs::rename(orig, to);
}

} // cwt

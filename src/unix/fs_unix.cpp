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
#include <dirent.h>

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

pfs::string fs::tempDirectory ()
{
	char * r = getenv("TMPDIR");

	if (!r) {
		r = getenv("TMP");
		if (!r) {
			r = getenv("TEMP");
			if (!r) {
				r = getenv("TEMPDIR");
			}
		}
	}

	return r ? _u8(r) : _u8("/tmp");
}

pfs::vector<pfs::string> fs::entryList (const pfs::string & dir, uint_t filters = NoFilter, uint_t sort = NoSort) const
{
	pfs::vector<pfs::string> r;

//#ifdef  _POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _BSD_SOURCE || _SVID_SOURCE || _POSIX_SOURCE
//#else
	struct dirent * d;
	DIR * dirp = opendir(dir.c_str());

	if (!dirp) {
		addSystemError(errno, _l1("opendir"));
		return pfs::vector<pfs::string>();
	}

	while ((d = readdir(dirp))) {
		struct stat st;
		pfs::string path;
		path << dir << separator() << _u8(d->d_name);
		stat(path.c_str(), &st );

		r << _u8(d->d_name);
	}

	closedir(dirp);
//#endif

	return r;
}

} // cwt

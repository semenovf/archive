/*
 * filesystem.cpp
 *
 *  Created on: Feb 15, 2013
 *      Author: wladt
 *
 */

#include "pfs/fs.hpp"
#include "pfs/regexp.hpp"
#include <sys/stat.h>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <dirent.h>

namespace pfs {

inline bool __is_regular_file  (mode_t m) { return (m & S_IFMT) == S_IFREG; }
inline bool __is_socket        (mode_t m) { return (m & S_IFMT) == S_IFSOCK; }
inline bool __is_symbolic_link (mode_t m) { return (m & S_IFMT) == S_IFLNK; }
inline bool __is_block_device  (mode_t m) { return (m & S_IFMT) == S_IFBLK; }
inline bool __is_directory     (mode_t m) { return (m & S_IFMT) == S_IFDIR; }
inline bool __is_char_device   (mode_t m) { return (m & S_IFMT) == S_IFCHR; }
inline bool __is_fifo          (mode_t m) { return (m & S_IFMT) == S_IFIFO; }

pfs::ucchar fs::separator () const
{
	return pfs::ucchar('/');
}

bool fs::isAbsolute(const pfs::string & path)
{
	if( path.isEmpty() )
		return false;

	return path.startsWith(pfs::string(1, separator()));
}

bool fs::isDirectory (const pfs::string & path)
{
	struct stat st;
	return (stat(path.c_str(), & st ) == 0
			&& __is_directory(st.st_mode));
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
	if (::unlink(path.c_str()) != 0) {
		pfs::string errstr;
		errstr << _Tr("Unable to unlink (delete) file ") << path;
		addSystemError(errno, errstr);
		return false;
	}
	return true;
}

size_t fs::size (const pfs::string & path) const
{
	struct stat st;
	return stat(path.c_str(), & st ) == 0
			? st.st_size
			: size_t(0);
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

// FIXME need to implement (Windows version too)
pfs::stringlist fs::entryListByRegExp (const pfs::string & dir, const pfs::stringlist & reNameFilters, uint_t filters, uint_t /*sort*/)
{
	pfs::stringlist r;

//#ifdef  _POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _BSD_SOURCE || _SVID_SOURCE || _POSIX_SOURCE
//#else
	struct dirent * d;
	DIR * dirp = opendir(dir.c_str());

	if (!dirp) {
		addSystemError(errno, _l1("opendir"));
		return pfs::stringlist();
	}

	while ((d = readdir(dirp))) {
		struct stat st;
		pfs::string fname(_u8(d->d_name));
		pfs::string path(join(dir, fname));

		if (stat(path.c_str(), & st ) != 0) {
			addSystemError(errno, _l1("stat"));
			return pfs::stringlist();
		}

		if (filters == NoFilter) {
			r.append(fname);
			continue;
		}

		// List all directories; i.e. don't apply the filters to directory names.
		//
		if ((filters & AllDirs) && __is_directory(st.st_mode)) {
			r.append(fname);
			continue;
		}

		if ((filters & NoSymLinks) && __is_symbolic_link(st.st_mode))
			continue;

		if (((filters & NoDotDot) || (filters & NoDotAndDotDot)) && fname.startsWith(".."))
			continue;

		if (((filters & NoDot) || (filters & NoDotAndDotDot)) && fname.startsWith("."))
			continue;

		if (!(filters & Hidden) && fname.startsWith("."))
			continue;

		// System files: FIFOs, sockets and device files
		//
		if (!(filters & System) && (__is_socket(st.st_mode)
				|| __is_block_device(st.st_mode)
				|| __is_char_device(st.st_mode)
				|| __is_fifo(st.st_mode)))
			continue;

		if (!(filters & Hidden) && fname.startsWith("."))
			continue;

		if (!(filters & Dirs) && __is_directory(st.st_mode))
			continue;

		if (!(filters & Files))
			continue;

		// Check name filters
		//
		for (pfs::stringlist::const_iterator it = reNameFilters.cbegin()
				; it != reNameFilters.cend()
				; ++it) {

			regexp re(*it);

			if (re.match(fname))
				r.append(fname);
		}
	}

	closedir(dirp);
//#endif

	return r;
}

pfs::stringlist fs::entryListByWildcard (const pfs::string & dir, const pfs::stringlist & nameFilters, uint_t filters, uint_t sort)
{
	pfs::stringlist reNameFilters;

	// Convert wildcards to regexp
	//
	for (pfs::stringlist::const_iterator it = nameFilters.cbegin()
			; it != nameFilters.cend()
			; ++it) {

		pfs::string restr(*it);
		restr.replace(_l1("."), _l1("\\."));
		restr.replace(_l1("*"), _l1(".*"));
		restr.replace(_l1("?"), _l1("."));
		reNameFilters.append(restr);
	}

	return entryListByRegExp(dir, reNameFilters, filters, sort);
}

} // pfs

/*
 * filesystem.cpp
 *
 *  Created on: Feb 15, 2013
 *      Author: wladt
 *
 */

#include "pfs/fs/path.hpp"
//#include "pfs/regexp.hpp"
#include <sys/stat.h>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <dirent.h>

namespace pfs { namespace fs {

path::path ()
	: _separator('/', 1)
{}

#if __COMMENT__
inline bool __is_regular_file  (mode_t m) { return (m & S_IFMT) == S_IFREG; }
inline bool __is_socket        (mode_t m) { return (m & S_IFMT) == S_IFSOCK; }
inline bool __is_symbolic_link (mode_t m) { return (m & S_IFMT) == S_IFLNK; }
inline bool __is_block_device  (mode_t m) { return (m & S_IFMT) == S_IFBLK; }
inline bool __is_directory     (mode_t m) { return (m & S_IFMT) == S_IFDIR; }
inline bool __is_char_device   (mode_t m) { return (m & S_IFMT) == S_IFCHR; }
inline bool __is_fifo          (mode_t m) { return (m & S_IFMT) == S_IFIFO; }


bool fs::isAbsolute (const string & path) const
{
	if( path.isEmpty() )
		return false;

	return path.startsWith(string(1, separator()));
}

bool fs::isDirectory (const string & path) const
{
	struct stat st;
	return (stat(path.c_str(), & st ) == 0
			&& __is_directory(st.st_mode));
}

bool fs::exists (const string & path) const
{
	struct stat st;
	return stat(path.c_str(), & st ) == 0;
}

bool fs::rename (const string & from, const string & to)
{
	if (::rename(from.c_str(), to.c_str()) != 0) {
		string errstr;
		errstr << _Tr("Unable to rename file")
				<< _Tr(" from ") << from
				<< _Tr(" to ") << to;
		addSystemError(errno, errstr);
		return false;
	}
	return true;
}

bool fs::remove (const string & path)
{
	if (::unlink(path.c_str()) != 0) {
		string errstr;
		errstr << _Tr("Unable to unlink (delete) file ") << path;
		addSystemError(errno, errstr);
		return false;
	}
	return true;
}

size_t fs::size (const string & path) const
{
	struct stat st;
	return stat(path.c_str(), & st ) == 0
			? st.st_size
			: size_t(0);
}

bool fs::simpleBackup (const string & orig)
{
	string to(orig);
	to.append(string(1, '~'));
	return fs::rename(orig, to);
}

string fs::currentDirectory () const
{
	string result;

#if _GNU_SOURCE
	char * str = get_current_dir_name();
	PFS_ASSERT(str);
	result = pfs::string::fromUtf8(str);
	free(str);
#else
	char buf[256];
	size_t size = 256;

	char * str = getcwd(buf, size);

	if (str)
		return pfs::string::fromUtf8(str);

	do {
		if (errno != ERANGE) {
			this->addSystemError(errno, _u8("Get current working directory"));
			break;
		}

		size *= 2;

		if (size > pfs::max_type<int16_t>()) {
			this->addError(_u8("Directory name too big"));
			break;
		}

		buf = new char[size];
		str = getcwd(buf, size);

		if (str)
			result = pfs::string::fromUtf8(str);

		delete [] buf;
	} while (!str);
#endif

	return result;
}

string fs::tempDirectory () const
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

// TODO need to implement (Windows version too)
stringlist fs::entryListByRegExp (const string & dir
		, const stringlist & reNameFilters
		, int filters, int /*sort*/)
{
	stringlist r;

//#ifdef  _POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _BSD_SOURCE || _SVID_SOURCE || _POSIX_SOURCE
//#else
	struct dirent * d;
	DIR * dirp = opendir(dir.c_str());

	if (!dirp) {
		addSystemError(errno, _l1("opendir"));
		return stringlist();
	}

	while ((d = readdir(dirp))) {
		struct stat st;
		string fname(_u8(d->d_name));
		string path(join(dir, fname));

		if (stat(path.c_str(), & st ) != 0) {
			addSystemError(errno, _l1("stat"));
			return stringlist();
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
		for (stringlist::const_iterator it = reNameFilters.cbegin()
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

stringlist fs::entryListByWildcard (const string & dir
		, const stringlist & nameFilters
		, int filters
		, int sort)
{
	stringlist reNameFilters;

	// Convert wildcards to regexp
	//
	for (stringlist::const_iterator it = nameFilters.cbegin()
			; it != nameFilters.cend()
			; ++it) {

		string restr(*it);
		restr.replace(_l1("."), _l1("\\."));
		restr.replace(_l1("*"), _l1(".*"));
		restr.replace(_l1("?"), _l1("."));
		reNameFilters.append(restr);
	}

	return entryListByRegExp(dir, reNameFilters, filters, sort);
}
#endif

}} // pfs::fs

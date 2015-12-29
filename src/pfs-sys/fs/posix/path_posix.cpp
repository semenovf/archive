/*
 * filesystem.cpp
 *
 *  Created on: Feb 15, 2013
 *      Author: wladt
 *
 */

#include <sys/stat.h>
#include <cstring>
#include <string>
#include <cerrno>
//#include <unistd.h>
//#include <dirent.h>
#include <pfs/algo/find.hpp>
#include "pfs/fs/path.hpp"

namespace pfs { namespace fs {

/*
inline bool __is_regular_file  (mode_t m) { return (m & S_IFMT) == S_IFREG;  }
inline bool __is_directory     (mode_t m) { return (m & S_IFMT) == S_IFDIR;  }
inline bool __is_symbolic_link (mode_t m) { return (m & S_IFMT) == S_IFLNK;  }
inline bool __is_block_device  (mode_t m) { return (m & S_IFMT) == S_IFBLK;  }
inline bool __is_char_device   (mode_t m) { return (m & S_IFMT) == S_IFCHR;  }
inline bool __is_fifo          (mode_t m) { return (m & S_IFMT) == S_IFIFO;  }
inline bool __is_socket        (mode_t m) { return (m & S_IFMT) == S_IFSOCK; }
*/

path::path ()
	: _separator(1, '/')
{}

path::path (const path::string_type & s)
	: _path(s)
	, _separator(1, '/')
{}

path::path (const char * s)
	: _path(s)
	, _separator(1, '/')
{}

path::path (const char * s, const char * separator)
	: _path(s)
	, _separator(separator)
{}

path & path::operator = (const string_type & s)
{
	_path = s;
	_separator = "/";
	return *this;
}

path & path::operator = (const char * s)
{
	_path = s;
	_separator = "/";
	return *this;
}

bool path::is_absolute () const
{
	if( _path.empty() )
		return false;

	return starts_with(_path.begin(), _path.end(), _separator.begin(), _separator.end());
}


file_status get_file_status (const path & p, error_code * ex)
{
	struct stat st;
	int rc = lstat(p.native().c_str(), & st);

	if (rc != 0) { // error
		if (rc == ENOENT || rc == ENOTDIR)
			return file_status(file_not_found);

		if (ex)
			*ex = rc;

		return file_status(status_error);
	}

	switch (st.st_mode & S_IFMT) {
	case S_IFREG : return file_status(regular_file);
	case S_IFDIR : return file_status(directory_file);
	case S_IFLNK : return file_status(symlink_file);
	case S_IFBLK : return file_status(block_file);
	case S_IFCHR : return file_status(character_file);
	case S_IFIFO : return file_status(fifo_file);
	case S_IFSOCK: return file_status(socket_file);
	}

	return file_status(type_unknown);
}

//inline bool __zero_error_is_ok (int errn)
//{
//	return errn == 0;
//}
//
//
//inline bool __native_call (int errn, bool (*predicate) (int), error_code * ex = 0)
//{
//	if (errn != 0) {
//		if (ex) {
//			path::string_type errstr;
//			nx->append(platform::strerror(errn, errstr));
//		}
//		return false;
//	}
//
//	return predicate(errn);
//}

uintmax_t file_size (const path & p, error_code * ex)
{
	struct stat st;
	int rc = lstat(p.native().c_str(), & st);

	if (rc == 0)
		return static_cast<uintmax_t>(st.st_size);

	if (ex) *ex = rc;

	return max_type<uintmax_t>();
}

bool remove (const path & p, error_code * ex)
{
	int rc = ::unlink(p.native().c_str());
	if (ex) *ex = rc;
	return rc == 0;
}

bool rename (const path & from, const path & to, error_code * ex)
{
	int rc = ::rename(from.native().c_str(), to.native().c_str());
	if (ex) *ex = rc;
	return rc == 0;
}

path temp_directory_path (error_code * ex)
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

	string s;

	if (r)
		s.append(r);
	else
		s.append("/tmp");

	return path(s);
}


path current_directory (error_code * ex)
{
	path result;

#if _GNU_SOURCE
	char * str = get_current_dir_name();
	PFS_ASSERT(str);
	result = path(str);
	free(str);
#else
	char buf[256];
	size_t size = 256;

	char * str = getcwd(buf, size);

	if (str)
		return path(str);

	do {
		if (errno != ERANGE) {
			if (ex)
				*ex = errno;
			break;
		}

		size *= 2;

		if (size > pfs::max_type<int16_t>()) {
			if (ex)
				*ex = ENOMEM;
			break;
		}

		buf = new char[size];
		str = getcwd(buf, size);

		if (str)
			result = path(str);

		delete [] buf;
	} while (!str);
#endif

	return result;
}


path join (const path & p1, const path & p2)
{
	string r(p1._path);
	r.append(p1._separator);
	r.append(p2._path);
	return path(r);
}

#if __COMMENT__

bool fs::simpleBackup (const string & orig)
{
	string to(orig);
	to.append(string(1, '~'));
	return fs::rename(orig, to);
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

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
#include <pfs/fs/path.hpp>
#include <pfs/platform/strerror.hpp>

namespace pfs { namespace fs {

inline bool __is_regular_file  (mode_t m) { return (m & S_IFMT) == S_IFREG;  }
inline bool __is_socket        (mode_t m) { return (m & S_IFMT) == S_IFSOCK; }
inline bool __is_symbolic_link (mode_t m) { return (m & S_IFMT) == S_IFLNK;  }
inline bool __is_block_device  (mode_t m) { return (m & S_IFMT) == S_IFBLK;  }
inline bool __is_directory     (mode_t m) { return (m & S_IFMT) == S_IFDIR;  }
inline bool __is_char_device   (mode_t m) { return (m & S_IFMT) == S_IFCHR;  }
inline bool __is_fifo          (mode_t m) { return (m & S_IFMT) == S_IFIFO;  }

path::path ()
	: _separator(1, '/')
{}

path::path (const path::string_type & s)
	: _path(s)
	, _separator(1, '/')
{}

bool path::is_absolute () const
{
	if( _path.empty() )
		return false;

	return starts_with(_path.begin(), _path.end(), _separator.begin(), _separator.end());
}

inline void __add_syserror_notification (int errn, notification_type * nx)
{
	if (errn != 0 && nx) {
		path::string_type errstr;
		nx->append(platform::strerror(errn, errstr));
	}
}

bool exists (const path & p, notification_type * nx)
{
	struct stat st;
	int rc = lstat(p.native().c_str(), & st);

	__add_syserror_notification(rc, nx);

	return rc == 0;
}

bool is_directory (const path & p, notification_type * nx)
{
	struct stat st;
	int rc = lstat(p.native().c_str(), & st);

	__add_syserror_notification(rc, nx);

	return (rc == 0 && __is_directory(st.st_mode));
}

bool remove (const path & p, notification_type * nx)
{
	int rc = ::unlink(p.native().c_str());

	__add_syserror_notification(rc, nx);

	return rc == 0;
}

bool rename (const path & from, const path & to, notification_type * nx)
{
	int rc = ::rename(from.native().c_str(), to.native().c_str());

	__add_syserror_notification(rc, nx);

	return rc == 0;
}

path temp_directory_path (notification_type * nx)
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

	return r ? path(path::string_type(r)) : path(path::string_type("/tmp"));
}

#if __COMMENT__

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

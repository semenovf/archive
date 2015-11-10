/*
 * path.hpp
 *
 *  Created on: Nov 7, 2015
 *      Author: wladt
 */

#ifndef __PFS_FS_PATH_HPP__
#define __PFS_FS_PATH_HPP__

#include <vector>
#include <pfs/shared_ptr.hpp>
#include <pfs/iterator.hpp>
#include <pfs/algo/split.hpp>
#include <pfs/platform/string.hpp>
#include <pfs/platform/notification.hpp>
#include <pfs/fs/file_status.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs { namespace fs {

typedef platform::notification_type notification_type;

class DLL_API path
{
public:
	typedef platform::string_type string_type;
	typedef typename std::vector<string_type> stringlist_type;

private:
	string_type _path;
	string_type _separator;
	shared_ptr<stringlist_type> _components;

public:

//	enum filter {
//		  NoFilter   = 0
//		, Dirs	     = 0x0001	  // List directories that match the filters.
//		, AllDirs	 = 0x0400	  // List all directories; i.e. don't apply the filters to directory names.
//		, Files	     = 0x0002	  // List files.
//		, Drives	 = 0x0004	  // List disk drives (ignored under Unix).
//		, NoSymLinks = 0x0008      // Do not list symbolic links (ignored by operating systems that don't support symbolic links).
//		, NoDotAndDotDot = 0x1000 // Do not list the special entries "." and "..".
//		, NoDot      = 0x2000     // Do not list the special entry ".".
//		, NoDotDot   = 0x4000     // Do not list the special entry "..".
//		, AllEntries = Dirs | Files | Drives // List directories, files, drives and symlinks (this does not list broken symlinks unless you specify System).
//		, Hidden     = 0x0100      // List hidden files (on Unix, files starting with a ".").
//		, System     = 0x0200      // List system files (on Unix, FIFOs, sockets and device files are included; on Windows, .lnk files are included)
//
//		// Use canRead | canWrite | canExec to check specific access of the application
//		//, Readable   = 0x0010      // List files for which the application has read access. The Readable value needs to be combined with Dirs or Files.
//		//, Writable   = 0x0020      // List files for which the application has write access. The Writable value needs to be combined with Dirs or Files.
//		//, Executable = 0x0040      // List files for which the application has execute access. The Executable value needs to be combined with Dirs or Files.
//		//, Modified   = 0x080      // Only list files that have been modified (ignored on Unix).
//		//, CaseSensitive = 0x800   // The filter should be case sensitive.
//	};

//	enum sort_order {
//		  NoSort      = -1    // Not sorted by default.
//		, Name        = 0x00  // Sort by name.
//		, Time        = 0x01  // Sort by time (modification time).
//		, Size        = 0x02  // Sort by file size.
//		, Type        = 0x80  // Sort by file type (extension).
//		, Unsorted    = 0x03  // Do not sort.
//		, DirsFirst   = 0x04  // Put the directories first, then the files.
//		, DirsLast    = 0x20  // Put the files first, then the directories.
//		, Reversed    = 0x08  // Reverse the sort order.
//		, IgnoreCase  = 0x10  // Sort case-insensitively.
//		, LocaleAware = 0x40  // Sort items appropriately using the current locale settings.
//	};

public:
	class iterator
	{
		friend class path;

	    typedef std::bidirectional_iterator_tag  iterator_category;
	    typedef typename stringlist_type::value_type      value_type;
	    typedef typename stringlist_type::difference_type difference_type;
	    typedef typename stringlist_type::iterator        pointer;
	    typedef typename stringlist_type::reference       reference;

		pointer _p;

		iterator (const pointer & ptr)
			: _p(ptr)
		{}

	public:
		iterator (const iterator & other)
			: _p(other._p)
		{}

		iterator & operator = (const iterator & other)
		{
			_p = other._p;
			return *this;
		}

		iterator & operator ++ ()
		{
	    	++_p;
	    	return *this;
		}

		//
		// input iterator traits
		//
		iterator operator ++ (int)
		{
	    	return iterator(_p++);
		}

		reference operator * ()
		{
			return *_p;
		}

		//
		// bidirectional iterator traits
	    //
	    iterator & operator -- ()
		{
	    	--_p;
	    	return *this;
		}

	    iterator operator -- (int)
		{
	    	return iterator(_p--);
		}

	    pointer base () const
	    {
	    	return _p;
	    }

		friend void swap (iterator & lhs, iterator & rhs)
		{
			std::swap(lhs._p, rhs._p);
		}

		friend bool operator == (const iterator & lhs, const iterator & rhs)
		{
			return lhs._p == rhs._p;
		}

	    friend bool operator != (const iterator & lhs, const iterator & rhs)
		{
	    	return lhs._p != rhs._p;
		}

		friend bool operator < (const iterator & lhs, const iterator & rhs)
		{
			return lhs._p < rhs._p;
		}

		friend bool operator <= (const iterator & lhs, const iterator & rhs)
		{
			return lhs._p <= rhs._p;
		}

		friend bool operator > (const iterator & lhs, const iterator & rhs)
		{
			return lhs._p > rhs._p;
		}

		friend bool operator >= (const iterator & lhs, const iterator & rhs)
		{
			return lhs._p >= rhs._p;
		}
	};

public:

	/**
	 * @brief Construct path object with empty path and platform-specific separator
	 *
	 * @note Platform specific.
	 */
	path ();

	path (const path & other)
		: _path(other._path)
		, _separator(other._separator)
	{}

	/**
	 * @brief Construct path object with path represented by string and platform-specific separator
	 * @param s String representation of path.
	 *
	 * @note Platform specific.
	 */
	path (const string_type & s);

	path (const string_type & s, const string_type & separator)
		: _path(s)
		, _separator(separator)
	{}

	/**
	 * @brief Returns path separator.
	 *
	 * @return Path separator
	 */
	const string_type & separator () const
	{
		return _separator;
	}

	/**
	 * @brief Sets path separator
	 *
	 * @param sep Path separator.
	 */
	void set_separator (const string_type & sep) { _separator = sep; }

	/**
	 * @brief Returns native representation of path.
	 *
	 * @return Native representation of path
	 */
	const string_type & native () const
	{
		return _path;
	}

	bool empty () const
	{
		return _path.empty();
	}

	iterator begin ()
	{
		if (!_components) {
			_components = make_shared<stringlist_type>();

			split(_path.begin(), _path.end()
					, _separator.begin(), _separator.end()
					, DontKeepEmpty, *_components);
		}

		return iterator(_components->begin());
	}

	iterator end ()
	{
		if (!_components) {
			_components = make_shared<stringlist_type>();

			split(_path.begin(), _path.end()
					, _separator.begin(), _separator.end()
					, DontKeepEmpty, *_components);
		}

		return iterator(_components->end());
	}

//	const_iterator begin () const;
//	const_iterator end () const;

	bool is_absolute () const;

	bool is_relative () const
	{
		return !is_absolute();
	}


//	size_t size (const string & path) const;
//	bool simple_backup (const string & path);
//	string currentDirectory () const;

//	string basename (const string & path) const;
//	string join (const string & dir, const string filename) const;
//	string join (const stringlist & items) const;
//	stringlist entryListByRegExp (const string & dir
//			, const stringlist & reNameFilters
//			, int filters
//			, int sort = NoSort);
//	stringlist entryListByWildcard (const string & dir
//			, const stringlist & nameFilters
//			, int filters = NoFilter, int sort = NoSort);
//
////	string findFile (const string & filename, const stringlist & dirs) const;
//	string normalizePath (const string & path) const;
};

/**
 * @brief Determines file type by path.
 * @param p Path.
 * @param nx Pointer to save error message(s).
 * @return File type (@see @a file_type_enum)
 */
file_status get_file_status (const path & p, notification_type * nx = 0);

/**
 * @brief Checks if @ path exists.
 * @param p Path.
 * @param nx Pointer to save error message(s).
 * @return @c true if @c path exists, @c false otherwise.
 *
 * @note Path exists if it's status is not equals to @c status_error nor @c file_not_found.
 */
inline bool exists (const path & p, notification_type * nx = 0)
{
	file_status ft = get_file_status(p, nx);
	return ft.type() != status_error && ft.type() != file_not_found;
}

/**
 * @brief Checks if @ path is regular file.
 *
 * @param p Path.
 * @param nx Pointer to save error message(s).
 * @return @c true if @c path is a regular file, @c false otherwise.
 */
inline bool is_regular_file (const path & p, notification_type * nx = 0)
{
	return get_file_status(p, nx).type() == regular_file;
}

inline bool is_directory (const path & p, notification_type * nx = 0)
{
	return get_file_status(p, nx).type() == directory_file;
}

inline bool is_symlink (const path & p, notification_type * nx = 0)
{
	return get_file_status(p, nx).type() == symlink_file;
}

uintmax_t file_size (const path & p, notification_type * nx = 0);

bool remove (const path & p, notification_type * nx = 0);

inline bool unlink (const path & p, notification_type * nx = 0)
{
	return remove(p, nx);
}

bool rename (const path & from, const path & to, notification_type * nx = 0);

path temp_directory_path (notification_type * nx = 0);

// FIXME Need to implement
path unique_path (const path & model, notification_type * nx = 0);

inline path unique_path ()
{
	return unique_path(path(path::string_type("%%%%-%%%%")), 0);
}

}} // pfs::fs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif


#endif /* __PFS_FS_PATH_HPP__ */

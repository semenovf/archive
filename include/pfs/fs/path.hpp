/*
 * path.hpp
 *
 *  Created on: Nov 7, 2015
 *      Author: wladt
 */

#ifndef __PFS_FS_PATH_HPP__
#define __PFS_FS_PATH_HPP__

#include <pfs/stringlist.hpp>
#include <pfs/shared_ptr.hpp>
#include <pfs/iterator.hpp>
#include <pfs/error_code.hpp>
#include <pfs/fs/file_status.hpp>
#include <pfs/mpl/algo/split.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs { namespace fs {

class path;
typedef pfs::vector<path> pathlist;

enum permission_enum
{
	  perm_user_read  = 00400 // User has read permission
	, perm_user_write = 00200 // User has write permission
	, perm_user_exec  = 00100 // User has execute permission
	, perm_user_all   = perm_user_read
	                  | perm_user_write | perm_user_exec // User (file owner) has read, write and execute permission

	, perm_group_read  = 00040 // Group has read permission
	, perm_group_write = 00020 // Group has write permission
	, perm_group_exec  = 00010 // Group has execute permission
	, perm_group_all   = perm_group_read  // Group has read, write and execute permission
					   | perm_group_write
					   | perm_group_exec

	, perm_other_read  = 00004 // Others have read permission
	, perm_other_write = 00002 // Others have write permission
	, perm_other_exec  = 00001 // Others have execute permission
	, perm_other_all   = perm_other_read  // Others have read, write and execute permission
					   | perm_other_write
					   | perm_other_exec
};

class DLL_API path
{
public:
	typedef pfs::string string_type;
	typedef pfs::stringlist stringlist_type;

public:
	static string_type default_separator ();
    static string_type separator ()
    {
        return default_separator();
    }

private:
	string_type _path;

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
		friend class range;

	    typedef std::bidirectional_iterator_tag           iterator_category;
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

	class range
	{
		friend class path;

		shared_ptr<stringlist_type> _pcomponents;

		range (string_type const & s, string_type const & separator = default_separator())
			: _pcomponents(new stringlist_type)
		{
			split(s, separator, pfs::mpl::dont_keep_empty, *_pcomponents);
		}

	public:
		iterator begin ()
		{
			return iterator(_pcomponents->begin());
		}

		iterator end ()
		{
			return iterator(_pcomponents->end());
		}
        
        /**
         * @return First component of the path or empty string if path is empty.
         */
        string_type first () const
        {
            return _pcomponents->size() > 0 ? _pcomponents->at(0) : string_type();
        }

        /**
         * @return Last component of the path empty string if path is empty.
         */
        string_type last () const
        {
            return _pcomponents->size() > 0 ? _pcomponents->at(_pcomponents->size() - 1) : string_type();
        }
	};

public:

	/**
	 * @brief Construct path object with empty path and platform-specific separator
	 *
	 * @note Platform specific.
	 */
	path ()
	{}

	path (const path & other)
		: _path(other._path)
	{}

	/**
	 * @brief Construct path object with path represented by string and platform-specific separator
	 * @param s String representation of path.
	 *
	 * @note Platform specific.
	 */
	explicit path (const string_type & s);

	path (const string_type & s, const string_type & separator)
		: _path(s)
	{}

	explicit path (const char * s);

	path & operator = (const string_type & s);
	path & operator = (const char * s);

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

	/**
	 * @brief Returns the canonical representation of this path.
	 */
	path canonical (const string_type & separator = default_separator()) const;

	range get_range (const string_type & separator = default_separator()) const
	{
		return range(_path, separator);
	}

	friend bool operator == (const path & lhs, const path & rhs)
	{
		return lhs._path == rhs._path;
	}

	friend bool operator != (const path & lhs, const path & rhs)
	{
		return lhs._path != rhs._path;
	}
};

/**
 * @brief Determines file type by path.
 * @param p Path.
 * @param nx Pointer to save error message(s).
 * @return File type (@see @a file_type_enum)
 */
file_status get_file_status (const path & p, error_code * ex = 0);

/**
 * @brief Checks if @ path exists.
 * @param p Path.
 * @param nx Pointer to save error message(s).
 * @return @c true if @c path exists, @c false otherwise.
 *
 * @note Path exists if it's status is not equals to @c status_error nor @c file_not_found.
 */
inline bool exists (const path & p, error_code * ex = 0)
{
	file_status ft = get_file_status(p, ex);
	return ft.type() != status_error && ft.type() != file_not_found;
}

/**
 * @brief Strip directory and suffix from path.
 * @param p Path to strip.
 * @return Path component with any leading directory components and suffixes removed.
 * 
 * The base name consists of all characters in the file up to (but not including) the first '.' character.
 */
string basename (path const & p);

/**
 * @brief Strip directory from path.
 * @param p Path to strip.
 * @return Path component with any leading directory components removed.
 */
inline string filename (path const & p)
{
    return p.get_range().last();
}

/**
 * @brief Checks if @ path is regular file.
 *
 * @param p Path.
 * @param nx Pointer to save error message(s).
 * @return @c true if @c path is a regular file, @c false otherwise.
 */
inline bool is_regular_file (const path & p, error_code * ex = 0)
{
	return get_file_status(p, ex).type() == regular_file;
}

inline bool is_directory (const path & p, error_code * ex = 0)
{
	return get_file_status(p, ex).type() == directory_file;
}

inline bool is_symlink (const path & p, error_code * ex = 0)
{
	return get_file_status(p, ex).type() == symlink_file;
}

uintmax_t file_size (const path & p, error_code * ex = 0);

bool remove (const path & p, error_code * ex = 0);

inline bool unlink (const path & p, error_code * ex = 0)
{
	return remove(p, ex);
}

bool rename (const path & from, const path & to, error_code * ex = 0);

path temp_directory_path (error_code * ex = 0);

// FIXME Need to implement
path unique_path (const path & model, error_code * ex = 0);

inline path unique_path ()
{
	return unique_path(path(path::string_type("%%%%-%%%%")), 0);
}

DLL_API path current_directory (error_code * ex = 0);

inline bool starts_with (const path & haystack, const path & needle)
{
	return pfs::fs::starts_with(haystack, needle);
}

/**
 * @brief Searches file in current directory and directories specified
 *        by @a searchdirs and returns appropriate existence path.
 *
 * @param file File name to search. May be absolute or relative.
 * @param searchdirs List of directories where to to search file.
 *
 * @return Path to file if found or empty path if @a filename is empty
 *         or file not found in list of directories specified by @a searchdirs.
 */
path search_file (const path & file, const pathlist & searchdirs, error_code * ex = 0);


/**
 * @brief Joins two path instances.
 *
 * @return Resulting path after join.
 */
path join (const path & p1, const path & p2, const string & separator = path::default_separator());

/**
 * @brief Joins path list components.
 *
 * @return Resulting path after join.
 */
path join (pathlist const & pl, string const & separator = path::default_separator());

/**
 * @brief Joins path list components and path instance.
 *
 * @return Resulting path after join.
 */
inline path join (pathlist const & pl, path const & p, string const & separator = path::default_separator())
{
	path r = join(pl, separator);
	return join(r, p, separator);
}

}} // pfs::fs


namespace pfs {

inline string to_string (const fs::path & p)
{
	return p.native();
}

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif


#endif /* __PFS_FS_PATH_HPP__ */

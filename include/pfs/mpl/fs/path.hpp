/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   path.hpp
 * Author: wladt
 *
 * Created on January 24, 2017, 9:56 AM
 */

#ifndef __PS_MPL_FS_PATH_HPP__
#define __PS_MPL_FS_PATH_HPP__

#include <pfs/iterator.hpp>
#include <pfs/mpl/algo/split.hpp>
#include <pfs/mpl/string_builder.hpp>
#include <pfs/mpl/fs/traits.hpp>
#include <pfs/mpl/fs/file_status.hpp>

namespace pfs {
namespace mpl {
namespace fs {

template <typename Traits>
class path
{
    typedef Traits traits_type;
    
public:
	typedef typename traits_type::string_type               string_type;
	typedef typename traits_type::stringlist_type           stringlist_type;
    typedef typename traits_type::string_builder_type       string_builder_type;
    typedef typename traits_type::template list<path>::type pathlist_type;

private:
	string_type _path;

public:

    static string_type separator ()
    {
        return traits_type::separator();
    }

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

//		friend void swap (iterator & lhs, iterator & rhs)
//		{
//			pfs::swap(lhs._p, rhs._p);
//		}

		friend bool operator == (const iterator & lhs, const iterator & rhs)
		{
			return lhs._p == rhs._p;
		}

	    friend bool operator != (const iterator & lhs, const iterator & rhs)
		{
	    	return lhs._p != rhs._p;
		}

//		friend bool operator < (const iterator & lhs, const iterator & rhs)
//		{
//			return lhs._p < rhs._p;
//		}
//
//		friend bool operator <= (const iterator & lhs, const iterator & rhs)
//		{
//			return lhs._p <= rhs._p;
//		}
//
//		friend bool operator > (const iterator & lhs, const iterator & rhs)
//		{
//			return lhs._p > rhs._p;
//		}
//
//		friend bool operator >= (const iterator & lhs, const iterator & rhs)
//		{
//			return lhs._p >= rhs._p;
//		}
	};

	class range
	{
		friend class path;

		stringlist_type * _pcomponents;

		range (string_type const & s)
			: _pcomponents(new stringlist_type)
		{
            pfs::mpl::split(s.cbegin()
                , s.cend()
                , separator().cbegin()
                , separator().cend()
                , keep_empty, _pcomponents);
		}

	public:
        ~range ()
        {
            delete _pcomponents;
        }
        
		iterator begin ()
		{
			return iterator(_pcomponents->begin());
		}

		iterator end ()
		{
			return iterator(_pcomponents->end());
		}
        
//        /**
//         * @return First component of the path or empty string if path is empty.
//         */
//        string_type first () const
//        {
//            return _pcomponents->size() > 0 ? _pcomponents->at(0) : string_type();
//        }
//
//        /**
//         * @return Last component of the path empty string if path is empty.
//         */
//        string_type last () const
//        {
//            return _pcomponents->size() > 0 ? _pcomponents->at(_pcomponents->size() - 1) : string_type();
//        }
	};

public:

	/**
	 * @brief Construct path object with empty path and platform-specific separator
	 *
	 * @note Platform specific.
	 */
	path ()
	{}

	path (path const & other)
		: _path(other._path)
	{}

	explicit path (string_type const & s)
        : _path(s)
    {}

	explicit path (char const * s)
        : _path(s)
    {}

	path & operator = (string_type const & s)
    {
        _path = s;
        return *this;
    }
    
	path & operator = (char const * s)
    {
        _path = s;
        return *this;
    }

	/**
	 * @brief Returns native representation of path.
	 *
	 * @return Native representation of path
	 */
	string_type const & native () const
	{
		return _path;
	}

	bool empty () const
	{
		return _path.empty();
	}

//	const_iterator begin () const;
//	const_iterator end () const;

	bool is_absolute () const
    {
        return _path.empty()
                ? false
                : _path.starts_with(separator());
    }

	bool is_relative () const
	{
		return !is_absolute();
	}

	/**
	 * @brief Returns the canonical representation of this path.
	 */
	path canonical () const;

	range get_range () const
	{
		return range(_path, separator());
	}

public: // static
    /**
     * @brief Joins two path instances.
     *
     * @return Resulting path after join.
     */
    static path join (path const & p1, path const & p2)
    {
        string_builder_type r;
        r.push_back(p1.native());
        r.push_back(separator());
        r.push_back(p2.native());
        return path(r.str());
    }

    /**
     * @brief Joins path list components.
     *
     * @return Resulting path after join.
     */
    static path join (pathlist_type const & pl);

    /**
     * @brief Joins path list components and path instance.
     *
     * @return Resulting path after join.
     */
    static path join (pathlist_type const & pl, path const & p)
    {
    	path r = join(pl, separator());
    	return join(r, p, separator());
    }
    
	friend bool operator == (path const & lhs, path const & rhs)
	{
		return lhs.canonical()._path == rhs.canonical()._path;
	}

	friend bool operator != (path const & lhs, path const & rhs)
	{
		return ! operator == (lhs, rhs);
	}
};

template <typename Traits>
path<Traits> path<Traits>::canonical () const
{
    stringlist_type comp;
	range rn(_path);
	iterator it = rn.begin();
	iterator it_end = rn.end();

	while (it != it_end) {
		if (*it == string_type(".")) {
			; // ignore
		} else if (*it == string_type("..")) {
			if (comp.size() > 0)
				comp.pop_front();
		} else {
			comp.push_front(*it);
		}

		++it;
	}

	pathlist_type pl;

	while (comp.size() > 0) {
		pl.push_back(path(comp.back()));
		comp.pop_back();
	}

//	if (this->is_absolute())
//		return join(path(), join(pl));

	return join(pl);
}

template <typename Traits>
path<Traits> path<Traits>::join (pathlist_type const & pl)
{
	string_builder_type r;

	typename pathlist_type::const_iterator it = pl.cbegin();
	typename pathlist_type::const_iterator it_end = pl.cend();

	if (it != it_end) {
//		r.push_back(it->native());
		++it;
	}

	while (it != it_end) {
//		r.push_back(separator());
//		r.push_back(it->native());
		++it;
	}

	return path();//(r.str<typename string_type>());
}

/**
 * @brief Determines file type by path.
 * @param p Path.
 * @param nx Pointer to save error message(s).
 * @return File type (@see @a file_type_enum)
 */
//file_status get_file_status (path const & p, error_code * ex = 0);

/**
 * @brief Checks if @ path exists.
 * @param p Path.
 * @param nx Pointer to save error message(s).
 * @return @c true if @c path exists, @c false otherwise.
 *
 * @note Path exists if it's status is not equals to @c status_error nor @c file_not_found.
 */
//inline bool exists (const path & p, error_code * ex = 0)
//{
//	file_status ft = get_file_status(p, ex);
//	return ft.type() != status_error && ft.type() != file_not_found;
//}

/**
 * @brief Strip directory and suffix from path.
 * @param p Path to strip.
 * @return Path component with any leading directory components and suffixes removed.
 * 
 * The base name consists of all characters in the file up to (but not including) the first '.' character.
 */
//string basename (path const & p);

/**
 * @brief Strip directory from path.
 * @param p Path to strip.
 * @return Path component with any leading directory components removed.
 */
//inline string filename (path const & p)
//{
//    return p.get_range().last();
//}

/**
 * @brief Checks if @ path is regular file.
 *
 * @param p Path.
 * @param nx Pointer to save error message(s).
 * @return @c true if @c path is a regular file, @c false otherwise.
 */
//inline bool is_regular_file (const path & p, error_code * ex = 0)
//{
//	return get_file_status(p, ex).type() == regular_file;
//}

//inline bool is_directory (const path & p, error_code * ex = 0)
//{
//	return get_file_status(p, ex).type() == directory_file;
//}
//
//inline bool is_symlink (const path & p, error_code * ex = 0)
//{
//	return get_file_status(p, ex).type() == symlink_file;
//}
//
//uintmax_t file_size (const path & p, error_code * ex = 0);
//
//bool remove (const path & p, error_code * ex = 0);
//
//inline bool unlink (const path & p, error_code * ex = 0)
//{
//	return remove(p, ex);
//}
//
//bool rename (const path & from, const path & to, error_code * ex = 0);
//
//path temp_directory_path (error_code * ex = 0);
//
//// FIXME Need to implement
//path unique_path (const path & model, error_code * ex = 0);
//
//inline path unique_path ()
//{
//	return unique_path(path(path::string_type("%%%%-%%%%")), 0);
//}
//
//path current_directory (error_code * ex = 0);
//
//inline bool starts_with (const path & haystack, const path & needle)
//{
//	return pfs::fs::starts_with(haystack, needle);
//}

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
//path search_file (const path & file, const pathlist & searchdirs, error_code * ex = 0);

//inline string to_string (const fs::path & p)
//{
//	return p.native();
//}

}}} // pfs::mpl::fs

#endif /* __PS_MPL_FS_PATH_HPP__ */


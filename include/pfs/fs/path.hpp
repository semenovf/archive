/*
 * path.hpp
 *
 *  Created on: Nov 7, 2015
 *      Author: wladt
 */

#ifndef __PFS_FS_PATH_HPP__
#define __PFS_FS_PATH_HPP__

#include <pfs/string.hpp>
#include <pfs/stringlist.hpp>
#include <pfs/notification.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs { namespace fs {

class DLL_API path
{
	string     _separator;
	stringlist _components;

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
	path ();
	path (const path & other)
		: _separator(other._separator)
		,  _components(other._components)
	{}

	path (const string & s);

	const string & separator () const { return _separator; }
	void set_separator (const string & sep) { _separator = sep; }

//	bool is_absolute (const string & path) const;
//	bool is_relative (const string & path) const { return !is_absolute(path); }
//	bool is_directory (const string & path) const;
//
//	bool exists (const string & path) const;
//	bool rename (const string & from, const string & to);
//	bool remove (const string & path);
//	bool unlink (const string & path) { return remove(path); }
//	size_t size (const string & path) const;
//	bool simple_backup (const string & path);
//	string currentDirectory () const;
//	string tempDirectory () const;
//	string tempFile (const string & prefix, const string & suffix, const string & dir, int nattempts = 10);
//	string tempFile (const string & prefix, const string & suffix, int nattempts = 10)
//	{
//	    return tempFile(prefix, suffix, tempDirectory(), nattempts);
//	}
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

}} // pfs::fs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif


#endif /* __PFS_FS_PATH_HPP__ */

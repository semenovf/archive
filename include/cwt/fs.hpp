/**
 * @file   fs.hpp
 * @author wladt
 * @date   Feb 15, 2013 9:02:52 AM
 *
 * @brief
 */

#ifndef __CWT_FS_HPP__
#define __CWT_FS_HPP__

#include <pfs/string.hpp>
#include <pfs/stringlist.hpp>
#include <cwt/errorable.hpp>

namespace cwt {

class fs : public errorable
{
public:
	enum filter {
		  NoFilter   = 0
		, Dirs	     = 0x0001	  // List directories that match the filters.
		, AllDirs	 = 0x0400	  // List all directories; i.e. don't apply the filters to directory names.
		, Files	     = 0x0002	  // List files.
		, Drives	 = 0x0004	  // List disk drives (ignored under Unix).
		, NoSymLinks = 0x0008      // Do not list symbolic links (ignored by operating systems that don't support symbolic links).
		, NoDotAndDotDot = 0x1000 // Do not list the special entries "." and "..".
		, NoDot      = 0x2000     // Do not list the special entry ".".
		, NoDotDot   = 0x4000     // Do not list the special entry "..".
		, AllEntries = Dirs | Files | Drives // List directories, files, drives and symlinks (this does not list broken symlinks unless you specify System).
		, Hidden     = 0x0100      // List hidden files (on Unix, files starting with a ".").
		, System     = 0x0200      // List system files (on Unix, FIFOs, sockets and device files are included; on Windows, .lnk files are included)

		// Use canRead | canWrite | canExec to check specific access of the application
		//, Readable   = 0x0010      // List files for which the application has read access. The Readable value needs to be combined with Dirs or Files.
		//, Writable   = 0x0020      // List files for which the application has write access. The Writable value needs to be combined with Dirs or Files.
		//, Executable = 0x0040      // List files for which the application has execute access. The Executable value needs to be combined with Dirs or Files.
		//, Modified   = 0x080      // Only list files that have been modified (ignored on Unix).
		//, CaseSensitive = 0x800   // The filter should be case sensitive.
	};

	enum sort_order {
		  NoSort      = -1    // Not sorted by default.
		, Name        = 0x00  // Sort by name.
		, Time        = 0x01  // Sort by time (modification time).
		, Size        = 0x02  // Sort by file size.
		, Type        = 0x80  // Sort by file type (extension).
		, Unsorted    = 0x03  // Do not sort.
		, DirsFirst   = 0x04  // Put the directories first, then the files.
		, DirsLast    = 0x20  // Put the files first, then the directories.
		, Reversed    = 0x08  // Reverse the sort order.
		, IgnoreCase  = 0x10  // Sort case-insensitively.
		, LocaleAware = 0x40  // Sort items appropriately using the current locale settings.
	};

public:
	fs () : errorable() {}
	pfs::ucchar separator ();
	bool isAbsolute (const pfs::string & path);
	bool isRelative (const pfs::string & path) { return !isAbsolute(path); }
	bool isDirectory (const pfs::string & path);
	bool exists (const pfs::string & path);
	bool rename (const pfs::string & from, const pfs::string & to);
	bool remove (const pfs::string & path);
	bool unlink (const pfs::string & path) { return remove(path); }
	bool simpleBackup (const pfs::string & path);
	pfs::string tempDirectory ();
	pfs::string join (const pfs::string & dir, const pfs::string filename);
	pfs::stringlist entryListByRegExp (const pfs::string & dir, const pfs::stringlist & reNameFilters, uint_t filters, uint_t sort = NoSort);
	pfs::stringlist entryListByWildcard (const pfs::string & dir, const pfs::stringlist & nameFilters, uint_t filters = NoFilter, uint_t sort = NoSort);
};

} // cwt

#endif /* __CWT_FS_HPP__ */

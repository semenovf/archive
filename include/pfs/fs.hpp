/**
 * @file   fs.hpp
 * @author wladt
 * @date   Feb 15, 2013 9:02:52 AM
 *
 * @brief
 */

#ifndef __PFS_FS_HPP__
#define __PFS_FS_HPP__

#include <pfs/string.hpp>
#include <pfs/stringlist.hpp>
#include <pfs/errorable_ext.hpp>

namespace pfs {


// XXX DEPRECATED. Use pfs::fs namespace instead
//
class DLL_API fs : public errorable_ext
{
	pfs::ucchar _separator;
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
	fs ();
	pfs::ucchar separator () const { return _separator; }
	pfs::ucchar separatorChar () const { return _separator; }
	void setSeparatorChar(char latin1) { _separator = ucchar(latin1); }
	void setSeparatorChar(pfs::ucchar ch) { _separator = ch; }

	bool isAbsolute (const string & path) const;
	bool isRelative (const string & path) const { return !isAbsolute(path); }
	bool isDirectory (const string & path) const;
	bool exists (const string & path) const;
	bool rename (const string & from, const string & to);
	bool remove (const string & path);
	bool unlink (const string & path) { return remove(path); }
	size_t size (const string & path) const;
	bool simpleBackup (const string & path);
	string currentDirectory () const;
	string tempDirectory () const;
	string tempFile (const string & prefix, const string & suffix, const string & dir, int nattempts = 10);
	string tempFile (const string & prefix, const string & suffix, int nattempts = 10)
	{
	    return tempFile(prefix, suffix, tempDirectory(), nattempts);
	}
	string basename (const string & path) const;
	string join (const string & dir, const string filename) const;
	string join (const stringlist & items) const;
	stringlist entryListByRegExp (const string & dir
			, const stringlist & reNameFilters
			, int filters
			, int sort = NoSort);
	stringlist entryListByWildcard (const string & dir
			, const stringlist & nameFilters
			, int filters = NoFilter, int sort = NoSort);

//	string findFile (const string & filename, const stringlist & dirs) const;
	string normalizePath (const string & path) const;
};

} // pfs

#endif /* __PFS_FS_HPP__ */

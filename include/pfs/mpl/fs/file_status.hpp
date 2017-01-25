/*
 * file_status.hpp
 *
 *  Created on: Nov 9, 2015
 *      Author: wladt
 */

#ifndef __PFS_MPL_FS_FILE_STATUS_HPP__
#define __PFS_MPL_FS_FILE_STATUS_HPP__

namespace pfs {
namespace mpl {
namespace fs {

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

enum file_type_enum
{
	  status_error
	, file_not_found
	, regular_file
	, directory_file
	, symlink_file
	, block_file
	, character_file
	, fifo_file
	, socket_file
	, type_unknown
};

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

class file_status
{
	file_type_enum _type;

public:
	file_status ()
		: _type(status_error)
	{}

	explicit file_status (file_type_enum type/*, perms prms = perms_not_known*/)
		: _type(type)
	{}

	file_type_enum type() const
	{
		return _type;
	}
    
//  TODO  
//	perms permissions() const;

	// modifiers
	void type (file_type_enum type)
	{
		_type = type;
	}

//	void permissions(perms prms);
};

}}} // pfs::mpl::fs

#endif /* __PFS_MPL_FS_FILE_STATUS_HPP__ */

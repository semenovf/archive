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

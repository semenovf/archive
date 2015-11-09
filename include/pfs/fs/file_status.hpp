/*
 * file_status.hpp
 *
 *  Created on: Nov 9, 2015
 *      Author: wladt
 */

#ifndef __PFS_FS_FILE_STATUS_HPP__
#define __PFS_FS_FILE_STATUS_HPP__

namespace pfs { namespace fs {

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
	file_type _type;

public:
	file_status ()
		: _type(status_error)
	{}

	explicit file_status (file_type type/*, perms prms = perms_not_known*/)
		: _type(type)
	{}

	file_type type() const
	{
		return _type;
	}
//	perms      permissions() const;

	// modifiers
	void type (file_type type)
	{
		_type = type;
	}

//	void permissions(perms prms);
};

}} // pfs::fs

#endif /* __PFS_FS_FILE_STATUS_HPP__ */

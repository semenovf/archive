/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   path.hpp
 * Author: wladt
 *
 * Created on January 25, 2017, 11:38 AM
 */

#ifndef __PFS_MPL_FS_POSIX_PATH_HPP__
#define __PFS_MPL_FS_POSIX_PATH_HPP__

#include <sys/stat.h>
#include <cerrno>
#include <pfs/mpl/fs/path.hpp>

namespace pfs {
namespace mpl {
namespace fs {

template <typename Traits>
file_status path<Traits>::status (path const & p)
{
	struct stat st;
	int rc = lstat(pfs::mpl::c_str(p.str())(), & st);

	if (rc != 0) { // error
		if (errno == ENOENT || errno == ENOTDIR)
			return file_status(file_not_found);

		return file_status(status_error);
	}

	switch (st.st_mode & S_IFMT) {
	case S_IFREG : return file_status(regular_file);
	case S_IFDIR : return file_status(directory_file);
	case S_IFLNK : return file_status(symlink_file);
	case S_IFBLK : return file_status(block_file);
	case S_IFCHR : return file_status(character_file);
	case S_IFIFO : return file_status(fifo_file);
	case S_IFSOCK: return file_status(socket_file);
	}

	return file_status(type_unknown);
}

}}}

#endif /* __PFS_MPL_FS_POSIX_PATH_HPP__ */


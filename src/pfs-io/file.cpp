/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <pfs/error_code.hpp>
#include <pfs/debug.hpp>
#include "pfs/io/file.hpp"

namespace pfs {

void file_appender::print (pfs::string const & msg)
{
	error_code ex;
	_d.write(msg.c_str(), msg.size(), & ex);
	_d.write("\n", 1);

	if (ex) {
		PFS_ERROR("Failed to write log message: " << to_string(ex));
	}
}

}
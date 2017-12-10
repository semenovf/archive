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

    PFS_ERROR2(ex == 0, _u8("Failed to write log message: ").append(to_string(ex)).c_str());
}

}
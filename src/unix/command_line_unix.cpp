/*
 * command_line_unix.cpp
 *
 *  Created on: May 29, 2015
 *      Author: wladt
 */

#include "pfs/command_line.hpp"

namespace pfs {

command_line::command_line ()
{
	setOptionFormat(option_format::UnixShortOption
			, option_format::UnixLongOption);
}

}

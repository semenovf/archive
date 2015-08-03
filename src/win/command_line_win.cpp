/*
 * command_line_win.cpp
 *
 *  Created on: May 29, 2015
 *      Author: wladt
 */

#include "pfs/command_line.hpp"

namespace pfs {

command_line::command_line ()
{
	setOptionFormat(option_format::WindowsShortOption
			, option_format::WindowsLongOption);
}

} // pfs


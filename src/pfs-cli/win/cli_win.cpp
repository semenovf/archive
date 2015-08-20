/*
 * cli_win.cpp
 *
 *  Created on: May 29, 2015
 *      Author: wladt
 */

#include "pfs/cli.hpp"

namespace pfs { namespace cli {

cli::cli ()
{
	allowShortOption(true);
	allowLongOption(true);
	setCaseSensitive(true);

	setShortOptionPrefix("/");
	setLongOptionPrefix("/");
	setShortOptargSeparator(":");
	setLongOptargSeparator(":");
}

}}


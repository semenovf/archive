/*
 * cli_unix.cpp
 *
 *  Created on: Aug 20, 2015
 *      Author: wladt
 */

#include "pfs/cli/cli.hpp"

namespace pfs { namespace cli {

cli::cli ()
{
	allowShortOption(true);
	allowLongOption(true);
	setCaseSensitive(true);

	setShortOptionPrefix("-");
	setLongOptionPrefix("--");
	setShortOptargSeparator(" ");
	setLongOptargSeparator("=");
}

}}

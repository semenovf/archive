/*
 * option.hpp
 *
 *  Created on: Aug 25, 2015
 *      Author: wladt
 */

#ifndef __PFS_CLI_OPTION_HPP__
#define __PFS_CLI_OPTION_HPP__

#include <pfs/variant.hpp>

namespace pfs { namespace cli {

class option
{
	typedef variant<bool, integral_t, real_t, string> data_type;
};

}}


#endif /* __PFS_CLI_OPTION_HPP__ */

/*
 * debby.hpp
 *
 *  Created on: Oct 6, 2015
 *      Author: wladt
 */

#ifndef __PFS_DEBBY_DEBBY_HPP__
#define __PFS_DEBBY_DEBBY_HPP__

#include <pfs/noncopyable.hpp>
#include <pfs/string.hpp>
#include "pfs/debby/database.hpp"

namespace pfs { namespace debby {

class debby : noncopyable
{
public:
	static database connect (const pfs::string & uristr, pfs::string * perrorstr = 0);
};

}} // pfs::debby

#endif /* __PFS_DEBBY_DEBBY_HPP__ */

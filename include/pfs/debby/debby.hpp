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
#include <pfs/errorable.hpp>
#include "database.hpp"

namespace pfs { namespace debby {

class debby : public errorable, noncopyable
{
public:
	debby ()
		: errorable()
	{}

	database connect (const pfs::string & uri);
};

}} // pfs::debby

#endif /* __PFS_DEBBY_DEBBY_HPP__ */

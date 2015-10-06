/*
 * database.hpp
 *
 *  Created on: Oct 5, 2015
 *      Author: wladt
 */

#ifndef __PFS_DEBBY_SQLITE3_DATABASE_HPP__
#define __PFS_DEBBY_SQLITE3_DATABASE_HPP__

#include <pfs/debby/database.hpp>
#include "sqlite3/sqlite3.h"

namespace pfs { namespace debby { namespace sqlite3 {

class database : public pfs::debby::database
{
	typedef sqlite3 * native_handle_type;
	native_handle_type _dbh_native;
};

}}} // pfs::debby::sqlite3

#endif // __PFS_DEBBY_SQLITE3_DATABASE_HPP__

/*
 * date_win.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#include <pfs/platform.hpp>
#include <windows.h>

namespace pfs { namespace platform {

DLL_API pfs::date current_date ()
{
    SYSTEMTIME t;
    memset(& t, 0, sizeof(SYSTEMTIME));
    GetLocalTime(& t);
    return pfs::date(t.wYear, t.wMonth, t.wDay);
}

}} // pfs::platform

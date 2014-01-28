/*
 * date_win.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#include "../../include/cwt/platform.hpp"
#include <windows.h>

namespace cwt { namespace platform {

DLL_API pfs::date currentDate ()
{
    SYSTEMTIME t;
    memset(& t, 0, sizeof(SYSTEMTIME));
    GetLocalTime(& t);
    return pfs::date(t.wYear, t.wMonth, t.wDay);
}

}} // cwt::platform

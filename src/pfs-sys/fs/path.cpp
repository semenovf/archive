/*
 * path.cpp
 *
 *  Created on: Nov 7, 2015
 *      Author: wladt
 */

#include <pfs/fs/path.hpp>

namespace pfs { namespace fs {

//path unique_path (const path & model, notification_type * nx)
//{
//    string r;
//    pfs::random rnd;
//
//    do {
//        string s;
//        uint32_t d = rnd.rand();
//        s << prefix << string::toString(d, 16) << suffix;
//        r = join(dir, s);
//    } while (exists(r) && --nattempts);
//
//    if (nattempts <= 0)
//        return string();
//
//    return r;
//}

}} // pfs::fs

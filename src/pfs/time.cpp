/*
 * time.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#include "pfs/time.hpp"
#include <cstring>

namespace pfs {

bool time::set_time (int hour, int min, int sec, int millis)
{
    if (valid(hour, min, sec, millis)) {
        _millis = (hour * SecondsPerHour + min * SecondsPerMinute + sec) * 1000 + millis;
        return true;
    }

    _millis = NullTime;
    return false;
}


time time::add_millis (int millis) const
{
    time r;
    if (valid()) {
        if (millis >= 0) {
        	r._millis = (_millis + millis) % MillisPerDay;
        } else {
            int ndays = (MillisPerDay - millis) / MillisPerDay;
            r._millis = (_millis + millis + ndays * MillisPerDay) % MillisPerDay;
        }
    }
    return r;
}

} // pfs




/*
 * time.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#include "../include/pfs/time.hpp"
#include <sstream>
#include <iomanip>

namespace pfs {

bool time::setTime (int hour, int min, int sec, int millis)
{
    if (isValid (hour, min, sec, millis)) {
        _millis = (hour * SecondsPerHour + min * SecondsPerMinute + sec) * 1000 + millis;
        return true;
    }

    _millis = NullTime;
    return false;
}


time time::addMillis (int millis) const
{
    time r;
    if (isValid()) {
        if (millis >= 0) {
        	r._millis = (_millis + millis) % MillisPerDay;
        } else {
            int ndays = (MillisPerDay - millis) / MillisPerDay;
            r._millis = (_millis + millis + ndays * MillisPerDay) % MillisPerDay;
        }
    }
    return r;
}


/**
 *
 *
 * @return
 */
string time::toString () const
{
	std::stringstream ss;
	ss << std::setw(2) << std::setfill('0') << hour()
	   << ':'
	   << std::setw(2) << std::setfill('0') << minute()
	   << ':'
	   << std::setw(2) << std::setfill('0') << second();
	return string::fromLatin1(ss.str());
}

} // pfs




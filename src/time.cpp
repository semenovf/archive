/*
 * time.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#include "../include/pfs/time.hpp"
#include <cstring>
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
 * @param format
 * @return
 *
 * %n     A newline character.
 * %t     A tab character.
 * %H     The hour as a decimal number using a 24-hour clock (range 00 to 23).
 * %I     The hour as a decimal number using a 12-hour clock (range 01 to 12).
 * %k     The hour (24-hour clock) as a decimal number (range 0 to 23); single digits are preceded by a blank.
 * %l     The hour (12-hour clock) as a decimal number (range 1 to 12); single digits are preceded by a blank.
 * %M     The minute as a decimal number (range 00 to 59).
 * %S     The second as a decimal number (range 00 to 60).
 * %R     The time in 24-hour notation (%H:%M).
 * %T     The time in 24-hour notation (%H:%M:%S).
 * %p     Either "AM" or "PM" according to the given time value, or the corresponding strings for the current locale.  Noon is treated as "PM" and midnight as "AM".
 */
string time::toString (const char * format) const
{
	std::stringstream ss;
	const char * p = format;
	const char * end = format + strlen(format);
	bool need_spec = false; // true if conversion specifier character expected

	while (p < end) {
		if (*p == '%') {
			if (need_spec) {
				ss << '%';
				need_spec = false;
			} else {
				need_spec = true;
			}
		} else {
			if (!need_spec) {
				ss << *p;
			} else {
				switch (*p) {
				case 'n':
					ss << std::endl;
					break;
				case 't':
					ss << '\t';
					break;
				case 'H':
					ss << std::setw(2) << std::setfill('0') << hour();
					break;
				case 'I':
					ss << std::setw(2) << std::setfill('0') << hour() % 12;
					break;
				case 'k':
					ss << std::setw(2) << std::setfill(' ') << hour();
					break;
				case 'l':
					ss << std::setw(2) << std::setfill(' ') << hour() % 12;
					break;
				case 'M':
					ss << std::setw(2) << std::setfill('0') << minute();
					break;
				case 'S':
					ss << std::setw(2) << std::setfill('0') << second();
					break;
				case 'R':
					ss << std::setw(2) << std::setfill('0') << hour()
					   << ':'
					   << std::setw(2) << std::setfill('0') << minute();
					break;
				case 'T':
					ss << std::setw(2) << std::setfill('0') << hour()
					   << ':'
					   << std::setw(2) << std::setfill('0') << minute()
					   << ':'
					   << std::setw(2) << std::setfill('0') << second();
					break;
				case 'p':
					ss << (hour() < 12 ? "AM" : "PM");
					break;
				default:
					ss << *p;
					break;
				}

				need_spec = false;
			}
		}
		++p;
	}
	return string::fromLatin1(ss.str().c_str());
}

/**
 *
 *
 * @return
 */
string time::toString () const
{
	return toString("%T"); // equivalent to %H:%M:%S
}

} // pfs




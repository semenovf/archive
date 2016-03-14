/*
 * time.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#include "pfs/time.hpp"

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

inline void append_prefixed2 (string & s, char fill_char, int i2)
{
	if (i2 >= 0 && i2 < 10) s.push_back(fill_char);
	s.append(to_string(i2));
}

inline void append_prefixed3 (string & s, char fill_char, int i3)
{
	if (i3 >= 0) {
		if (i3 < 100) s.push_back(fill_char);
		if (i3 < 10) s.push_back(fill_char);
	}
	s.append(to_string(i3));
}

string to_string (const time & t, const string & format)
{
	string r;

	typename string::const_iterator p = format.begin();
	typename string::const_iterator end = format.end();

	bool need_spec = false; // true if conversion specifier character expected

	while (p < end) {
		if (*p == '%') {
			if (need_spec) {
				r.push_back('%');
				need_spec = false;
			} else {
				need_spec = true;
			}
		} else {
			if (!need_spec) {
				r.push_back(*p);
			} else {
				switch (lexical_cast<char>(*p)) {
				case 'n':
					r.push_back('\n');
					break;
				case 't':
					r.push_back('\t');
					break;
				case 'H':
					append_prefixed2(r, '0', t.hour());
					break;
				case 'I':
					append_prefixed2(r, '0', t.hour() % 12);
					break;
				case 'k':
					append_prefixed2(r, ' ', t.hour());
					break;
				case 'l':
					append_prefixed2(r, ' ', t.hour() % 12);
					break;
				case 'M':
					append_prefixed2(r, '0', t.minute());
					break;
				case 'q':
					r.append(to_string(t.millis()));
					break;
				case 'Q':
					append_prefixed3(r, '0', t.millis());
					break;
				case 'S':
					append_prefixed2(r, '0', t.second());
					break;
				case 'R':
					append_prefixed2(r, '0', t.hour());
					r.push_back(':');
					append_prefixed2(r, '0', t.minute());
					break;
				case 'T':
					append_prefixed2(r, '0', t.hour());
					r.push_back(':');
					append_prefixed2(r, '0', t.minute());
					r.push_back(':');
					append_prefixed2(r, '0', t.second());
					break;
				case 'J':
					append_prefixed2(r, '0', t.hour());
					r.push_back(':');
					append_prefixed2(r, '0', t.minute());
					r.push_back(':');
					append_prefixed2(r, '0', t.second());
					r.push_back('.');
					append_prefixed3(r, '0', t.millis());
					break;
				case 'p':
					r.append(t.hour() < 12 ? _u8("AM") : _u8("PM"));
					break;
				default:
					r.push_back('%');
					r.push_back(*p);
					break;
				}

				need_spec = false;
			}
		}
		++p;
	}

	return r;
}

} // pfs




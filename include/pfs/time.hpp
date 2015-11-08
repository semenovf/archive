/*
 * time.hpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#ifndef __PFS_TIME_HPP__
#define __PFS_TIME_HPP__

#include <pfs.hpp>
#include <sstream>
#include <iomanip>

namespace pfs {

class DLL_API time
{
	int _millis; // milliseconds from midnight

public:
	static const int NullTime         = -1;
	static const int SecondsPerMinute = 60;
	static const int SecondsPerHour   = 3600;
	static const int SecondsPerDay    = 86400;
    static const int MillisPerMinute  = 60000;
    static const int MillisPerHour    = 3600000;
    static const int MillisPerDay     = 86400000;

public:
	time () : _millis(NullTime) {}
	time (int hour, int min, int sec = 0, int millis = 0) { set_time(hour, min, sec, millis); }

	time add_millis (int millis) const;
	time add_seconds   (int secs) const { return add_millis(secs * 1000); }

	int	hour () const;
	int	minute () const;
	int	second () const;
	int	millis () const;

	bool valid () const;

	int	millis_to (const time & t) const;
	int	seconds_to (const time & t) const;

	bool set_time (int hour, int min, int sec, int millis = 0);

	bool operator == (const time & t) const { return _millis == t._millis; }
	bool operator != (const time & t) const { return _millis != t._millis; }
	bool operator  < (const time & t) const { return _millis  < t._millis; }
	bool operator <= (const time & t) const { return _millis <= t._millis; }
	bool operator  > (const time & t) const { return _millis  > t._millis; }
	bool operator >= (const time & t) const { return _millis >= t._millis; }

	static bool valid (int hour, int min, int sec, int millis = 0);
	static int millis (int hour, int min, int sec, int millis = 0);
};


inline bool time::valid (int hour, int min, int sec, int millis) // static
{
	return static_cast<unsigned int>(hour) < 24
			&& static_cast<unsigned int>(min) < 60
			&& static_cast<unsigned int>(sec) < 60
			&& static_cast<unsigned int>(millis) < 1000;
}

inline int time::millis (int hour, int min, int sec, int millis) // static
{
	return MillisPerHour * hour + MillisPerMinute * min + 1000 * sec + millis;
}

inline bool time::valid () const
{
	return _millis > NullTime && _millis < MillisPerDay;
}

inline int time::hour () const
{
	return valid() ? _millis / MillisPerHour : NullTime;
}

inline int time::minute () const
{
	return valid() ? (_millis % MillisPerHour) / MillisPerMinute : NullTime;
}

inline int time::second () const
{
	return valid() ? (_millis / 1000) % SecondsPerMinute : NullTime;
}

inline int time::millis () const
{
	return valid() ? _millis % 1000 : NullTime;
}

inline int time::millis_to (const time & t) const
{
	return valid() && t.valid() ? t._millis - _millis : 0;
}

inline int time::seconds_to(const time & t) const
{
    return valid() &&  t.valid()
    		? int(t._millis / 1000) - int(_millis / 1000)
    		: 0;
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
template <typename String>
String & lexical_cast (const time & t, const String & format, String & result)
{
	std::basic_stringstream<typename String::value_type> ss;

	typename String::const_iterator p = format.begin();
	typename String::const_iterator end = format.end();

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
					ss << std::setw(2) << std::setfill('0') << t.hour();
					break;
				case 'I':
					ss << std::setw(2) << std::setfill('0') << t.hour() % 12;
					break;
				case 'k':
					ss << std::setw(2) << std::setfill(' ') << t.hour();
					break;
				case 'l':
					ss << std::setw(2) << std::setfill(' ') << t.hour() % 12;
					break;
				case 'M':
					ss << std::setw(2) << std::setfill('0') << t.minute();
					break;
				case 'S':
					ss << std::setw(2) << std::setfill('0') << t.second();
					break;
				case 'R':
					ss << std::setw(2) << std::setfill('0') << t.hour()
					   << ':'
					   << std::setw(2) << std::setfill('0') << t.minute();
					break;
				case 'T':
					ss << std::setw(2) << std::setfill('0') << t.hour()
					   << ':'
					   << std::setw(2) << std::setfill('0') << t.minute()
					   << ':'
					   << std::setw(2) << std::setfill('0') << t.second();
					break;
				case 'p':
					ss << (t.hour() < 12 ? "AM" : "PM");
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

	result = ss.str();
	return result;
}

template <typename String>
String & lexical_cast (const time & t, String & result)
{
	return lexical_cast(t, String("%T"), result); // equivalent to %H:%M:%S
}

} // pfs

#endif /* __PFS_TIME_HPP__ */

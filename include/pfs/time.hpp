/*
 * time.hpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#ifndef __PFS_TIME_HPP__
#define __PFS_TIME_HPP__

#include <pfs/string.hpp>

namespace pfs {

class DLL_API time
{
	int32_t _millis; // milliseconds from midnight

public:
	static const int NullTime         = -1;
	static const int SecondsPerMinute = 60;
	static const int SecondsPerHour   = 3600;
	static const int SecondsPerDay    = 86400;
    static const int MillisPerMinute  = 60000;
    static const int MillisPerHour    = 3600000;
    static const int MillisPerDay     = 86400000;

public:
	time ()
		: _millis(NullTime)
	{}

	time (int hour, int min, int sec = 0, int millis = 0)
	{
		set_time(hour, min, sec, millis);
	}

	int32_t millis_from_midnight () const
	{
		return _millis;
	}

	time add_millis  (int millis) const;
	time add_seconds (int secs) const { return add_millis(secs * 1000); }

	int	hour () const
	{
		return valid() ? _millis / MillisPerHour : NullTime;
	}

	int	minute () const
	{
		return valid() ? (_millis % MillisPerHour) / MillisPerMinute : NullTime;
	}

	int	second () const
	{
		return valid() ? (_millis / 1000) % SecondsPerMinute : NullTime;
	}

	int	millis () const
	{
		return valid() ? _millis % 1000 : NullTime;
	}

	bool valid () const
	{
		return _millis > NullTime && _millis < MillisPerDay;
	}

	int	millis_to (const time & t) const
	{
		return valid() && t.valid()
			? t._millis - _millis
			: 0;
	}

	int	seconds_to (const time & t) const
	{
	    return valid() &&  t.valid()
			? int(t._millis / 1000) - int(_millis / 1000)
			: 0;
	}

	bool set_time (int hour, int min, int sec, int millis = 0);

	bool operator == (const time & t) const
	{
		return _millis == t._millis;
	}

	bool operator != (const time & t) const
	{
		return _millis != t._millis;
	}

	bool operator  < (const time & t) const
	{
		return _millis  < t._millis;
	}

	bool operator <= (const time & t) const
	{
		return _millis <= t._millis;
	}

	bool operator  > (const time & t) const
	{
		return _millis  > t._millis;
	}

	bool operator >= (const time & t) const
	{
		return _millis >= t._millis;
	}

	static bool valid (int hour, int min, int sec, int millis = 0)
	{
		return static_cast<unsigned int>(hour) < 24
				&& static_cast<unsigned int>(min) < 60
				&& static_cast<unsigned int>(sec) < 60
				&& static_cast<unsigned int>(millis) < 1000;
	}

	static int millis (int hour, int min, int sec, int millis = 0)
	{
		return MillisPerHour * hour + MillisPerMinute * min + 1000 * sec + millis;
	}
};

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
 * %q     The milliseconds (range 000 to 999).
 * %Q     The milliseconds (range 0 to 999).
 * %S     The second as a decimal number (range 00 to 60).
 * %R     The time in 24-hour notation (%H:%M).
 * %T     The time in 24-hour notation (%H:%M:%S).
 * %J     The time in 24-hour notation with milliseconds part (%H:%M:%S.%Q).
 * %p     Either "AM" or "PM" according to the given time value, or the corresponding strings for the current locale.  Noon is treated as "PM" and midnight as "AM".
 */
string to_string (const time & t, const string & format);

/**
 *
 * @param t
 * @return
 */
inline string to_string (const time & t)
{
	return to_string(t, string("%T"));
}

DLL_API pfs::time current_time ();

} // pfs

#endif /* __PFS_TIME_HPP__ */

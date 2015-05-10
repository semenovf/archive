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
	time (int hour, int min, int sec = 0, int millis = 0) { setTime(hour, min, sec, millis); }

	time addMillis (int millis) const;
	time addSecs   (int secs) const { return addMillis(secs * 1000); }

	int	hour () const;
	int	minute () const;
	int	second () const;
	int	millis () const;

	bool isValid () const;

	int	millisTo (const time & t) const;
	int	secsTo (const time & t) const;

	bool setTime (int hour, int min, int sec, int millis = 0);
	string toString (const char * format) const;
	string toString () const;

	bool operator == (const time & t) const { return _millis == t._millis; }
	bool operator != (const time & t) const { return _millis != t._millis; }
	bool operator  < (const time & t) const { return _millis  < t._millis; }
	bool operator <= (const time & t) const { return _millis <= t._millis; }
	bool operator  > (const time & t) const { return _millis  > t._millis; }
	bool operator >= (const time & t) const { return _millis >= t._millis; }

	static bool isValid (int hour, int min, int sec, int millis = 0);
	static int millis (int hour, int min, int sec, int millis = 0);
};


inline bool time::isValid (int hour, int min, int sec, int millis) // static
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

inline bool time::isValid () const
{
	return _millis > NullTime && _millis < MillisPerDay;
}

inline int time::hour () const
{
	return isValid() ? _millis / MillisPerHour : NullTime;
}

inline int time::minute () const
{
	return isValid() ? (_millis % MillisPerHour) / MillisPerMinute : NullTime;
}

inline int time::second () const
{
	return isValid() ? (_millis / 1000) % SecondsPerMinute : NullTime;
}

inline int time::millis () const
{
	return isValid() ? _millis % 1000 : NullTime;
}

inline int time::millisTo (const time & t) const
{
	return isValid() && t.isValid() ? t._millis - _millis : 0;
}

inline int time::secsTo(const time & t) const
{
    return isValid() &&  t.isValid()
    		? int(t._millis / 1000) - int(_millis / 1000)
    		: 0;
}

} // pfs

#endif /* __PFS_TIME_HPP__ */

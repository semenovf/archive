/*
 * time.hpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#ifndef __CWT_TIME_HPP__
#define __CWT_TIME_HPP__

#include <cwt/string.hpp>
#include <cwt/hash.hpp>

CWT_NS_BEGIN

class DLL_API Time
{
public:
	static const int NullTime         = -1;
	static const int SecondsPerMinute = 60;
	static const int SecondsPerHour   = 3600;
	static const int SecondsPerDay    = 86400;
    static const int MillisPerMinute  = 60000;
    static const int MillisPerHour    = 3600000;
    static const int MillisPerDay     = 86400000;

public:
	Time () : m_millis(NullTime) {}
	Time (int hour, int min, int sec = 0, int millis = 0) { setTime(hour, min, sec, millis); }

	Time addMillis (int millis) const;
	Time addSecs   (int secs) const { return addMillis(secs * 1000); }

	int	hour () const;
	int	minute () const;
	int	second () const;
	int	millis () const;

	bool isValid () const;

	int	millisTo (const Time & t) const;
	int	secsTo (const Time & t) const;

	bool setTime (int hour, int min, int sec, int millis = 0);
	String toString () const;

	bool operator == (const Time & t) const { return m_millis == t.m_millis; }
	bool operator != (const Time & t) const { return m_millis != t.m_millis; }
	bool operator  < (const Time & t) const { return m_millis  < t.m_millis; }
	bool operator <= (const Time & t) const { return m_millis <= t.m_millis; }
	bool operator  > (const Time & t) const { return m_millis  > t.m_millis; }
	bool operator >= (const Time & t) const { return m_millis >= t.m_millis; }

	static Time	currentTime ();
	static bool isValid (int hour, int min, int sec, int millis = 0);
	static int millis (int hour, int min, int sec, int millis = 0);

private:
	int m_millis; // milliseconds from midnight
};


inline bool Time::isValid (int hour, int min, int sec, int millis) // static
{
	return uint_t(hour) < 24 && uint_t(min) < 60 && uint_t(sec) < 60 && uint_t(millis) < 1000;
}

inline int Time::millis (int hour, int min, int sec, int millis) // static
{
	return MillisPerHour * hour + MillisPerMinute * min + 1000 * sec + millis;
}

inline bool Time::isValid () const
{
	return m_millis > NullTime && m_millis < MillisPerDay;
}

inline int Time::hour () const
{
	return isValid() ? m_millis / MillisPerHour : NullTime;
}

inline int Time::minute () const
{
	return isValid() ? (m_millis % MillisPerHour) / MillisPerMinute : NullTime;
}

inline int Time::second () const
{
	return isValid() ? (m_millis / 1000) % SecondsPerMinute : NullTime;
}

inline int Time::millis () const
{
	return isValid() ? m_millis % 1000 : NullTime;
}

inline int Time::millisTo (const Time & t) const
{
	return isValid() && t.isValid() ? t.m_millis - m_millis : 0;
}

inline int Time::secsTo(const Time & t) const
{
    return isValid() &&  t.isValid()
    		? int(t.m_millis / 1000) - int(m_millis / 1000)
    		: 0;
}

inline uint_t hash_func(const Time & key, uint_t seed)
{
	return hash_func(Time(0, 0, 0).millisTo(key), seed);
}

CWT_NS_END

#endif /* __CWT_TIME_HPP__ */

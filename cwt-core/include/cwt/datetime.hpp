/*
 * datetime.hpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#ifndef __CWT_DATETIME_HPP__
#define __CWT_DATETIME_HPP__

#include <cwt/date.hpp>
#include <cwt/time.hpp>
#include <cwt/hash.hpp>
#include <cwt/string.hpp>
#include <cwt/shared_ptr.hpp>

CWT_NS_BEGIN

class DLL_API DateTime
{
public:
	DateTime ();
	DateTime (const Date & date);
	DateTime (const Date & date, const Time & time);

	DateTime(const DateTime & other) : m_d(other.m_d) {}
	DateTime & operator = (const DateTime & other);

	DateTime addDays   (int ndays) const;
	DateTime addMonths (int nmonths) const;
	DateTime addYears  (int nyears) const;
	DateTime addMillis (long_t millis) const;
	DateTime addSecs   (long_t secs) const;

	long_t daysTo   (const DateTime & other) const;
	long_t secsTo   (const DateTime & other) const;
	long_t millisTo (const DateTime & other) const;
	long_t millisSinceEpoch () const;

	Date date () const { return m_d->date; }
	Time time () const { return m_d->time; }
	void setDate (const Date & date);
	void setTime (const Time & time);
	void setMillisSinceEpoch (long_t millis);

	bool isValid () const { return m_d->date.isValid() && m_d->time.isValid(); }
	String toString () const;

	bool operator == (const DateTime & other) const;
	bool operator != (const DateTime & other) const { return ! (*this == other); }
	bool operator  < (const DateTime & other) const;
	bool operator <= (const DateTime & other) const { return ! (other < *this); }
	bool operator  > (const DateTime & other) const { return other < *this; }
	bool operator >= (const DateTime & other) const { return ! (*this < other); }

	static DateTime fromMillisSinceEpoch (long_t millis);
	static DateTime currentDateTime ();

private:
	struct SharedData
	{
		Date date;
		Time time;

		SharedData () : date(), time(0, 0, 0) {}
		SharedData (const Date & d) : date(d), time(0, 0, 0) {}
		SharedData (const Date & d, const Time & t) : date(d), time(t) {
			time = d.isValid() && !t.isValid() ? Time(0, 0, 0) : t;
		}
	};

	cwt::shareable<SharedData> m_d;
};


inline DateTime::DateTime ()
	: m_d (new DateTime::SharedData)
{
}

inline DateTime::DateTime (const Date & date)
	: m_d (new DateTime::SharedData(date))
{
}

inline DateTime::DateTime (const Date & date, const Time & time)
	: m_d (new DateTime::SharedData(date, time))
{
}


inline DateTime & DateTime::operator = (const DateTime & other)
{
	m_d = other.m_d;
	return *this;
}

inline DateTime DateTime::addDays(int ndays) const
{
    return DateTime(m_d->date.addDays(ndays), m_d->time);
}

inline DateTime DateTime::addMonths(int nmonths) const
{
    return DateTime(m_d->date.addMonths(nmonths), m_d->time);
}

inline DateTime DateTime::addYears(int nyears) const
{
    return DateTime(m_d->date.addYears(nyears), m_d->time);
}

inline DateTime DateTime::addSecs(long_t secs) const
{
    return addMillis(secs * 1000);
}

inline long_t DateTime::daysTo(const DateTime &other) const
{
    return m_d->date.daysTo(other.m_d->date);
}

inline bool DateTime::operator == (const DateTime & other) const
{
	return m_d->time == other.m_d->time
			&& m_d->date == other.m_d->date;
}

inline bool DateTime::operator < (const DateTime & other) const
{
	return m_d->date != other.m_d->date
			? m_d->date < other.m_d->date
			: m_d->time < other.m_d->time;
}

inline uint_t hash_func(const DateTime & key, uint_t seed)
{
	return hash_func(key.millisSinceEpoch(), seed);
}


CWT_NS_END

#endif /* __CWT_DATETIME_HPP__ */

/*
 * notification.hpp
 *
 *  Created on: Nov 7, 2015
 *      Author: wladt
 */

#ifndef __PFS_NOTIFICATION_HPP__
#define __PFS_NOTIFICATION_HPP__

#include <pfs/ostream.hpp>
#include <pfs/string.hpp>
#include <pfs/vector.hpp>

namespace pfs {

enum notification_type_enum
{
	  notification_trace
	, notification_debug
	, notification_info
	, notification_warn
	, notification_error
	, notification_fatal

	, notification_count
};

class notification_value
{
public:
	typedef string string_type;

private:
	size_t _repetitions;
	string_type _text;
	notification_type_enum _type;

public:
	notification_value ()
		: _repetitions(0)
		, _text()
		, _type(notification_error)
	{}

	notification_value (const string_type & s)
		: _repetitions(1)
		, _text(s)
		, _type(notification_error)
	{}

	notification_value (notification_type_enum nxtype, const string_type & s)
		: _repetitions(1)
		, _text(s)
		, _type(nxtype)
	{}

	notification_value (const notification_value & other)
		: _repetitions(other._repetitions)
		, _text(other._text)
		, _type(other._type)
	{}

	notification_type_enum type () const
	{
		return _type;
	}

	const string & text () const
	{
		return _text;
	}

	size_t repetitions () const
	{
		return _repetitions;
	}

	void increment ()
	{
		++_repetitions;
	}
};

struct notification_traits
{
	typedef string string_type;
	typedef notification_value value_type;
	typedef typename vector<value_type>::type container_type;
	typedef typename container_type::iterator iterator;
	typedef typename container_type::const_iterator const_iterator;
};

class notification
{
public:
	typedef notification_traits traits_type;
	typedef typename traits_type::string_type string_type;
	typedef typename traits_type::value_type value_type;
	typedef typename traits_type::container_type container_type;
	typedef typename traits_type::iterator iterator;
	typedef typename traits_type::const_iterator const_iterator;

protected:
	container_type _notifications;
	size_t         _type_counts[notification_count];

private:
	notification (const notification & other);
	notification & operator == (const notification & other);

public:
	notification ()
	{
		for (int i = 0; i < notification_count; ++i)
			_type_counts[i] = 0;
	}

	virtual ~notification () { }

	void append (const string_type & text)
	{
		++_type_counts[notification_error];
		append(notification_error, text);
	}

	void append (notification_type_enum type, const string_type & text);

	void append (const notification & other);

	void clear ()
	{
		_notifications.clear();
		for (int i = 0; i < notification_count; ++i)
			_type_counts[i] = 0;
	}

	/**
	 * @brief Returns number of notifications (repetitions treated as single)
	 * @return Number of notifications.
	 */
	size_t count () const
	{
		return _notifications.size();
	}

	size_t count_type (notification_type_enum nxtype)
	{
		return nxtype < notification_count
				? _type_counts[nxtype]
				: 0;
	}

	size_t count_error_type ()
	{
		return _type_counts[notification_error] + _type_counts[notification_fatal];
	}

	iterator begin ()
	{
		return _notifications.begin();
	}

	const_iterator begin () const
	{
		return _notifications.begin();
	}

	iterator end ()
	{
		return _notifications.end();
	}

	const_iterator end () const
	{
		return _notifications.end();
	}

	const_iterator cbegin () const
	{
		return _notifications.begin();
	}

	const_iterator cend () const
	{
		return _notifications.end();
	}

	const string_type & last_text () const
	{
		PFS_ASSERT(_notifications.size() > 0);
		return _notifications.back().text();
	}

	size_t last_repetitions () const
	{
		PFS_ASSERT(_notifications.size() > 0);
		return _notifications.back().repetitions();
	}
};

} // pfs

std::ostream & operator << (std::ostream & os, const pfs::notification & nx);

#endif /* __PFS_NOTIFICATION_HPP__ */

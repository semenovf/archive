/*
 * notification.hpp
 *
 *  Created on: Nov 7, 2015
 *      Author: wladt
 */

#ifndef __PFS_NOTIFICATION_HPP__
#define __PFS_NOTIFICATION_HPP__

#include <memory> // for std::allocator
#include <ostream>
#include <pfs/string.hpp>
#include <pfs/vector.hpp>

namespace pfs {

template <typename String>
class notification_value
{
public:
	typedef String string_type;

private:
	size_t _repetitions;
	string_type _text;

public:
	notification_value () : _repetitions(0), _text() {}
	notification_value (const string_type & s) : _repetitions(1), _text(s) {}
	notification_value (const notification_value & other)
		: _repetitions(other._repetitions)
		, _text(other._text)
	{}

	const String & text () const { return _text; }
	size_t repetitions () const { return _repetitions; }
	void increment () { ++_repetitions; }
};

//template <typename String, template <typename, typename = std::allocator<notification_value<String> > > typename Container>
//struct notification_traits
//{
//	typedef String string_type;
//	typedef notification_value<String> value_type;
//	typedef Container<value_type, std::allocator<value_type> > container_type;
//	typedef typename container_type::iterator iterator;
//	typedef typename container_type::const_iterator const_iterator;
//};

template <typename String>
struct notification_traits
{
	typedef String string_type;
	typedef notification_value<String> value_type;
	typedef std::vector<value_type> container_type;
	typedef typename container_type::iterator iterator;
	typedef typename container_type::const_iterator const_iterator;
};

template <typename String>
class basic_notification
{
public:
	typedef notification_traits<String> traits_type;
	typedef typename traits_type::string_type string_type;
	typedef typename traits_type::value_type value_type;
	typedef typename traits_type::container_type container_type;
	typedef typename traits_type::iterator iterator;
	typedef typename traits_type::const_iterator const_iterator;

protected:
	container_type _notifications;

private:
	basic_notification (const basic_notification & other);
	basic_notification & operator == (const basic_notification & other);

public:
	basic_notification () {}
	virtual ~basic_notification () { }

	void append (const string_type & text);
	void concat (const basic_notification & other) { _notifications.append(other._notifications); }

	void clear ()
	{
		_notifications.clear();
	}

	/**
	 * @brief Returns number of notifications (repetitions treated as single)
	 * @return Number of notifications.
	 */
	size_t count () const
	{
		return _notifications.size();
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

#if __cplusplus >= 201103L
	const_iterator cbegin () const
	{
		return _notifications.cbegin();
	}

	const_iterator cend () const
	{
		return _notifications.cend();
	}
#endif

	bool is_error () const
	{
		return count() == 0;
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

template <typename String>
void basic_notification<String>::append (const string_type & text)
{
	if (!text.empty()) {
		if (_notifications.empty())
			_notifications.push_back(value_type(text));
		else if (last_text() == text)
			_notifications.back().increment();
		else
			_notifications.push_back(value_type(text));
	}
}


template <typename String>
std::ostream & operator << (std::ostream & out, const basic_notification<String> & nx)
{
    if (nx.count() > 0) {
    	typename basic_notification<String>::const_iterator it = nx.begin();
    	typename basic_notification<String>::const_iterator itEnd = nx.end();

        for (; it != itEnd; ++it) {
            if (it->repetitions() > 1) {
                out << it->text() << _u8(" (repeat ") << it->repetitions() << _u8(" times)") << std::endl;
            } else {
                out << it->text() << std::endl;
            }
        }
    }

    return out;
}


typedef basic_notification<string> notification;

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_NOTIFICATION_HPP__ */

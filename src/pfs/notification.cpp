/*
 * notification.cpp
 *
 *  Created on: Dec 28, 2015
 *      Author: wladt
 */

#include "pfs/notification.hpp"

namespace pfs {

void notification::append (notification_type_enum nxtype, const string_type & text)
{
	if (!text.empty()) {
		if (_notifications.empty())
			_notifications.push_back(value_type(nxtype, text));
		else if (last_text() == text)
			_notifications.back().increment();
		else
			_notifications.push_back(value_type(nxtype, text));

		++_type_counts[nxtype];
	}
}

void notification::append (const notification & other)
{
	container_type::const_iterator it = _notifications.begin();
	container_type::const_iterator it_end = _notifications.end();

	for (; it != it_end; ++it) {
		_notifications.push_back(*it);
	}

	for (int i = 0; i < notification_count; ++i)
		_type_counts[i] += other._type_counts[i];
}

} // pfs

std::ostream & operator << (std::ostream & os, const pfs::notification & nx)
{
    if (nx.count() > 0) {
    	typename pfs::notification::const_iterator it = nx.begin();
    	typename pfs::notification::const_iterator itEnd = nx.end();

        for (; it != itEnd; ++it) {
            if (it->repetitions() > 1) {
                os << it->text()
                   << _u8(" (repeat ")
                   << it->repetitions()
                   << _u8(" times)")
                   << std::endl;
            } else {
                os << it->text() << std::endl;
            }
        }
    }

    return os;
}



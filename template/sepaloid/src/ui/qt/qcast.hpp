/**
 * @file qcast.hpp
 * @author wladt
 * @date Feb 27, 2014
 */

#ifndef __QCAST_HPP__
#define __QCAST_HPP__

#include <pfs/string.hpp>
#include <QString>

inline QString qcast (const pfs::string & s) { return QString::fromUtf8(s.utf8()); }
inline pfs::string qcast (const QString & s) { return pfs::string::fromUtf8(s.toUtf8()); }

#endif /* __QCAST_HPP__ */

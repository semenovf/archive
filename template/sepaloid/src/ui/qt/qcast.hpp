/**
 * @file qcast.hpp
 * @author
 * @date
 */

#ifndef __QCAST_HPP__
#define __QCAST_HPP__

#include <pfs/string.hpp>
#include <QString>

inline QString qcast (const pfs::string & s) { return QString::fromUtf8(s.toUtf8().c_str()); }
inline pfs::string qcast (const QString & s) { return pfs::string::fromUtf8(s.toUtf8()); }

#endif /* __QCAST_HPP__ */

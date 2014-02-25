/**
 * @file Application.hpp
 * @author wladt
 * @date Feb 19, 2014
 */

#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include <QApplication>
#include <QString>
#include <pfs/string.hpp>

inline QString qcast (const pfs::string & s) { return QString(s.utf8()); }
inline pfs::string qcast (const QString & s) { return pfs::string::fromUtf8(s.toUtf8()); }

class Application : public QApplication
{
	pfs::string _stencilDir;
public:
	Application (int & argc, char ** argv);

	pfs::string symbolDirectory () const { return _stencilDir; }
};

#endif /* __APPLICATION_HPP__ */

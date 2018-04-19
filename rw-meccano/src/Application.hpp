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

class Application : public QApplication
{
	pfs::string _stencilDir;
public:
	Application (int & argc, char ** argv);

	pfs::string symbolDirectory () const { return _stencilDir; }
};

#endif /* __APPLICATION_HPP__ */

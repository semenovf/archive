/**
 * @file Application.hpp
 * @author wladt
 * @date Feb 19, 2014
 */

#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include <QApplication>

class Application : public QApplication
{
	pfs::string _symbolDir;
public:
	Application (int & argc, char ** argv);

	pfs::string symbolDir () const { return _symbolDir; }
};

#endif /* __APPLICATION_HPP__ */

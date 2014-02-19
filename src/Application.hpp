/**
 * @file Application.hpp
 * @author wladt
 * @date Feb 19, 2014
 */

#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include <QApplication>
#include "SchemeDom.hpp"
#include <errno.h>

class Application : public QApplication
{
	SchemeDom _scheme;

public:
	Application (int & argc, char ** argv);

	const SchemeDom & scheme () const { return _scheme; }
	SchemeDom & scheme () { return _scheme; }
};

#endif /* __APPLICATION_HPP__ */

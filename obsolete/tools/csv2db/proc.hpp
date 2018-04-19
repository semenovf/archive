/*
 * proc.hpp
 *
 *  Created on: Aug 26, 2013
 *      Author: wladt
 */

#ifndef __PROC_HPP__
#define __PROC_HPP__

#include <cwt/string.hpp>
#include <cwt/app.hpp>

using namespace cwt;

class Csv2DbProc : public cwt::MainProc
{
public:
	Csv2DbProc(const String & sourcePath) : cwt::MainProc() , _sourcePath(sourcePath) {}
	virtual int operator () () { return proc(); }
	int proc();

private:
	String _sourcePath;
};

#endif /* __PROC_HPP__ */

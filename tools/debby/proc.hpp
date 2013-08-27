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

class DebbyProc : public cwt::MainProc
{
public:
	DebbyProc(const String & path = String()) : cwt::MainProc(), m_debbyFilePath(path) {}
	virtual int operator () () { return proc(); }
	int proc();

private:
	String m_debbyFilePath;
};

#endif /* __PROC_HPP__ */

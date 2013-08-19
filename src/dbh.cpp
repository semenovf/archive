/*
 * dbh.cpp
 *
 *  Created on: Aug 19, 2013
 *      Author: wladt
 */

#include "../include/cwt/dbh.hpp"
#include <cwt/dl.hpp>

CWT_NS_BEGIN

bool DbHandler::loadDriver (const String & name)
{
	String path = Dl::buildDlFileName(name);
	Dl::Handle dlh = Dl::open(path);
}

CWT_NS_END



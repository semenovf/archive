/**
 * @file Application.cpp
 * @author wladt
 * @date Feb 19, 2014
 */

#include "Application.hpp"
#include <pfs/string.hpp>

Application::Application (int & argc, char ** argv)
	: QApplication (argc, argv)
{
	if (argc > 1) {
		_symbolDir = _u8(argv[1]);
	} else {
		_symbolDir = _l1("rc/rw-meccano");
	}
}


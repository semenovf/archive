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
		_scheme.loadFromFile(_u8(argv[1]));
	} else {
		_scheme.loadFromFile(_l1("rw-meccano.svg"));
	}

	if (_scheme.isError()) {
		_scheme.logErrors();
	}
}


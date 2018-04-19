/**
 * @file Application.cpp
 * @author wladt
 * @date Feb 19, 2014
 */

#include "Application.hpp"
#include "Stencil.hpp"
#include <pfs/string.hpp>

Application::Application (int & argc, char ** argv)
	: QApplication (argc, argv)
{
	if (argc > 1) {
		_stencilDir = _u8(argv[1]);
	} else {
		_stencilDir = _l1("rc/rw-meccano");
	}

	Stencil::loadStencils(_stencilDir);
}


/**
 * @file   abnf.c
 * @author wladt
 * @date   Oct 31, 2012 15:54:00
 *
 * @brief
 */


#include <cwt/abnf.hpp>
#include <cwt/logger.hpp>
#include <iostream>

using namespace cwt;

extern String generateAbnfTransitions();

int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	std::cout << generateAbnfTransitions();

	return EXIT_SUCCESS;
}


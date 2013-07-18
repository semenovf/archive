/*
 * options.cpp
 *
 *  Created on: Jun 4, 2013
 *      Author: wladt
 */

#include "../include/cwt/options.hpp"

CWT_NS_BEGIN

const char* OptionsReader::DEFAULT_SHORT_PREFIX     = "-";
const char* OptionsReader::DEFAULT_LONG_PREFIX      = "--";
const char* OptionsReader::DEFAULT_OPTARG_SEPARATOR = "=";

bool OptionsReader::parse(int argc, char *argv[])
{
	CWT_UNUSED2(argc, argv);
	return false;
}

CWT_NS_END

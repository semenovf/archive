/*
 * options.cpp
 *
 *  Created on: Jun 4, 2013
 *      Author: wladt
 */

#include "../include/cwt/options.hpp"

CWT_NS_BEGIN

const String OptionsReader::DEFAULT_SHORT_PREFIX     = "-";
const String OptionsReader::DEFAULT_LONG_PREFIX      = "--";
const String OptionsReader::DEFAULT_OPTARG_SEPARATOR = "=";

bool OptionsReader::parse(Settings & settings, int argc, char *argv[], size_t optc, const Options * optv)
{
	CWT_UNUSED(settings);
	CWT_UNUSED2(argc, argv);
	CWT_UNUSED2(optv, optc);
	return false;
}

CWT_NS_END

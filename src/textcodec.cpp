/**
 * @file   textcodec.cpp
 * @author wladt
 * @date   Mar 1, 2013 11:08:46 AM
 *
 * @brief
 */

#include "../include/cwt/textcodec.hpp"
#include "utfcodec_p.hpp"
#include <cwt/mt.h>
#include <cstring>

CWT_NS_BEGIN

#ifndef CWT_SINGLE_THREADED
static mt_def(g_mutex);
#endif

TextCodec* TextCodec::forName(const char* name)
{
	TextCodec *codec = NULL;

	// Search in list of standard codecs
	if (strcasecmp(name, "utf8") == 0
			|| strcasecmp(name, "utf-8") == 0) {
		return new Utf8TextCodec;
	}

	return codec;
}

CWT_NS_END

/*
 * app.cpp
 *
 *  Created on: Aug 15, 2013
 *      Author: wladt
 */


#include "../include/cwt/app.hpp"

CWT_NS_BEGIN

App * App::self = nullptr;

App::App(int argc, char * argv[], size_t optc, const Options * optv)
{
	CWT_CHECK_SIZEOF_TYPES;
	CWT_ASSERT(self == nullptr);
	self = this;

	OptionsReader optreader;
	(void)optreader.parse(m_settings, argc, argv, optc, optv);
}

CWT_NS_END

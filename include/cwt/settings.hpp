/*
 * settings.hpp
 *
 *  Created on: Jun 4, 2013
 *      Author: wladt
 */

#ifndef __CWT_SETTINGS_HPP__
#define __CWT_SETTINGS_HPP__

#include <cwt/pimpl.hpp>
#include <cwt/unitype.hpp>

CWT_NS_BEGIN

class Settings {
	CWT_PIMPL_IMPL(Settings);
public:
	Settings();
	void setValue(const String& path, const UniType& value);
};

CWT_NS_END

#endif /* __CWT_SETTINGS_HPP__ */

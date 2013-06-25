/*
 * settings.cpp
 *
 *  Created on: Jun 4, 2013
 *      Author: wladt
 *
 * JSON implementation of Settings class.
 */

#include "../include/cwt/settings.hpp"
#include "../include/cwt/json.hpp"

CWT_NS_BEGIN

class Settings::Impl : public Json {
public:
	Impl() : Json() {}
};

Settings::Settings() : pimpl(new Settings::Impl) {}

void Settings::setValue(const String& path, const UniType& value)
{
	CWT_UNUSED2(path, value);
}

CWT_NS_END

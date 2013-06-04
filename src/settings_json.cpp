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

const Char Settings::DEFAULT_SEPARATOR_CHAR = Char('/');

class Settings::Impl : public Json {
public:
	Impl() : Json(), m_separatorChar(Settings::DEFAULT_SEPARATOR_CHAR) {}

public:
	Char m_separatorChar;
};

Settings::Settings() : pimpl(new Settings::Impl)
{
}

void Settings::setPathSeparator(Char separator)
{
	pimpl->m_separatorChar = separator;
}

static Vector<UniType> split_path(const String & key)
{
	Vector<UniType> path;

	return path;
}

void Settings::setValue(const String & key, const UniType & value)
{

}

CWT_NS_END

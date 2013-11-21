/*
 * settings.cpp
 *
 *  Created on: Oct 31, 2013
 *      Author: wladt
 */

#include "settings_impl.hpp"
#include "../include/cwt/dl.hpp"

CWT_NS_BEGIN

class Settings::Impl : public SettingsImpl
{};

static const char * __plugin_basenames[] = {
	"cwt-settings-json"
};

Settings::Settings(Format format) : pimpl(new Settings::Impl), _format(format)
{
	String path = Dl::buildDlFileName(__plugin_basenames[format]);
	CWT_ASSERT(Dl::pluginOpen(__plugin_basenames[format], path, this->pimpl.get()));
}

Settings::~Settings()
{
	Dl::pluginClose(__plugin_basenames[_format], this->pimpl.get());
}

bool Settings::parse(const String & str) { return pimpl->parse(this->pimpl.get(), str); }

void Settings::set (const String & path, bool value)          { pimpl->setBool   (this->pimpl.get(), path, value); }
void Settings::set (const String & path, double value)        { pimpl->setDouble (this->pimpl.get(), path, value); }
void Settings::set (const String & path, float value)         { pimpl->setFloat  (this->pimpl.get(), path, value); }
void Settings::set (const String & path, long_t value)        { pimpl->setLong   (this->pimpl.get(), path, value); }
void Settings::set (const String & path, ulong_t value)       { pimpl->setULong  (this->pimpl.get(), path, value); }
void Settings::set (const String & path, int_t value)         { pimpl->setInt    (this->pimpl.get(), path, value); }
void Settings::set (const String & path, uint_t value)        { pimpl->setUInt   (this->pimpl.get(), path, value); }
void Settings::set (const String & path, const String & s)    { pimpl->setString (this->pimpl.get(), path, s); }
void Settings::set (const String & path, const char * latin1) { pimpl->setLatin1 (this->pimpl.get(), path, latin1); }

UniType Settings::value (const String & path, const UniType & defaultValue) const
	{ return pimpl->value(this->pimpl.get(), path, defaultValue); }

bool Settings::boolean (const String & path, bool defaultValue) const
	{ return pimpl->boolean(this->pimpl.get(), path, defaultValue); }

long_t Settings::integer (const String & path, long_t defaultValue) const
	{ return pimpl->integer(this->pimpl.get(), path, defaultValue); }

double  Settings::number (const String & path, double defaultValue) const
	{ return pimpl->number(this->pimpl.get(), path, defaultValue); }

String Settings::string (const String & path, const String & defaultValue) const
	{ return pimpl->string(this->pimpl.get(), path, defaultValue); }

Vector<UniType> Settings::array (const String & path, const Vector<UniType> & defaultValue) const
	{ return pimpl->array(this->pimpl.get(), path, defaultValue); }

bool Settings::contains (const String & path) const
	{ return pimpl->contains(this->pimpl.get(), path); }

UniType::Type Settings::type (const String & path) const
	{ return pimpl->type(this->pimpl.get(), path); }

CWT_NS_END



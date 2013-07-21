/**
 * settings_json.cpp
 *
 *  Created on: Jun 26, 2013
 *      Author: wladt
 *
 * @brief Settings implementation based on JSON.
 */

#include "../include/cwt/settings.hpp"
#include <cwt/json.hpp>
#include <cwt/vector.hpp>

CWT_NS_BEGIN

class Settings::Impl : public JsonSimplePath
{
public:
	Impl() : JsonSimplePath(*(new Json())) {  }
	~Impl() { delete & json(); }
};

Settings::Settings() : pimpl(new Settings::Impl) { ; }
void Settings::setValue(const String& path, bool value)         { pimpl->setValue(path, value); }
void Settings::setValue(const String& path, double value)       { pimpl->setValue(path, value); }
void Settings::setValue(const String& path, float value)        { pimpl->setValue(path, value); }
void Settings::setValue(const String& path, long_t value)       { pimpl->setValue(path, value); }
void Settings::setValue(const String& path, ulong_t value)      { pimpl->setValue(path, value); }
void Settings::setValue(const String& path, int_t value)        { pimpl->setValue(path, value); }
void Settings::setValue(const String& path, uint_t value)       { pimpl->setValue(path, value); }
void Settings::setValue(const String& path, const String & s)   { pimpl->setValue(path, s); }
void Settings::setValue(const String& path, const Char * chars) { pimpl->setValue(path, String(chars)); }
void Settings::setValue(const String& path, const Char * chars, size_t size) { pimpl->setValue(path, String(chars, size)); }
void Settings::setValue(const String& path, const char * utf8)  { pimpl->setValue(path, String::fromUtf8(utf8)); }
void Settings::setValue(const String& path, const char * utf8, size_t size) { pimpl->setValue(path, String::fromUtf8(utf8, size)); }

UniType Settings::value(const String& path, const UniType & defaultValue)
{
	JsonValue & jvalue = pimpl->find(path);

	if (jvalue.isInvalid() || jvalue.isObject() || jvalue.isArray())
		return UniType(defaultValue);

	return jvalue.value();
}

bool Settings::boolean (const String & path, bool defaultValue)
{
	JsonValue & jvalue = pimpl->find(path);

	if (jvalue.isInvalid() || jvalue.isObject() || jvalue.isArray())
		return defaultValue;

	return jvalue.boolean();
}

long_t Settings::integer (const String & path, long_t defaultValue)
{
	JsonValue & jvalue = pimpl->find(path);

	if (jvalue.isInvalid() || jvalue.isObject() || jvalue.isArray())
		return defaultValue;

	return jvalue.integer();
}

double Settings::number (const String & path, double defaultValue)
{
	JsonValue & jvalue = pimpl->find(path);

	if (jvalue.isInvalid() || jvalue.isObject() || jvalue.isArray())
		return defaultValue;

	return jvalue.number();
}

String Settings::string (const String & path, const String & defaultValue)
{
	JsonValue & jvalue = pimpl->find(path);

	if (jvalue.isInvalid() || jvalue.isObject() || jvalue.isArray())
		return defaultValue;

	return jvalue.string();
}

Vector<UniType> Settings::array (const String & path, const Vector<UniType> & defaultValue)
{
	JsonValue & jvalue = pimpl->find(path);

	if (jvalue.isInvalid() || jvalue.isObject()) {
		return Vector<UniType>(defaultValue);
	}

	if (!jvalue.isArray()) {
		return Vector<UniType>(1, jvalue.value());
	}

	Vector<UniType> arr;
	size_t size = jvalue.size();

	for (size_t i = 0; i < size; ++i)
		if (jvalue[i].isScalar())
			arr.append(jvalue[i].value());

	return arr;
}

bool Settings::parse(const String & str, Format format)
{
	if (format == UnknownFormat) {
		// try native format - JSON
		return pimpl->json().parse(str);
	}

	switch(format) {
	case JsonFormat: return pimpl->json().parse(str);
	default: break;
	}

	return false;
}

bool Settings::parseFromFile(const char *path, Format format)
{
	if (format == UnknownFormat) {
		// try native format - JSON
		return pimpl->json().parseFromFile(path);
	}

	switch(format) {
	case JsonFormat: return pimpl->json().parseFromFile(path);
	default: break;
	}

	return false;

}

CWT_NS_END

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
#include <cwt/safeformat.hpp>
#include <cwt/logger.hpp>
#include <cwt/io/file.hpp>
#include <cwt/io/textreader.hpp>

CWT_NS_BEGIN

class Settings::Impl : public JsonSimplePath
{
public:
	Impl() : JsonSimplePath(*(new Json())) {  }
	~Impl() { delete & json(); }
};

Settings::Settings() : pimpl(new Settings::Impl) { ; }
void Settings::setValue(const String& path, bool value)          { pimpl->setValue(path, value); }
void Settings::setValue(const String& path, double value)        { pimpl->setValue(path, value); }
void Settings::setValue(const String& path, float value)         { pimpl->setValue(path, value); }
void Settings::setValue(const String& path, long_t value)        { pimpl->setValue(path, value); }
void Settings::setValue(const String& path, ulong_t value)       { pimpl->setValue(path, value); }
void Settings::setValue(const String& path, int_t value)         { pimpl->setValue(path, value); }
void Settings::setValue(const String& path, uint_t value)        { pimpl->setValue(path, value); }
void Settings::setValue(const String& path, const String & s)    { pimpl->setValue(path, s); }
void Settings::setValue(const String& path, const char * latin1) { pimpl->setValue(path, latin1); }

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

bool Settings::parseFromFile(const String & path, Format format)
{
	shared_ptr<io::File> file(new io::File);

	if (!file->open(path, io::Device::ReadOnly)) {
		Logger::error(_Fr("Failed to open file: %s") % path);
		return false;
	}
	io::TextReader reader(dynamic_pointer_cast<io::Device, io::File>(file));
	String s(reader.read(file->available()));
	file->close();
	return parse(s, format);
}

CWT_NS_END

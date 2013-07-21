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
#include <cwt/vector.hpp>
#include <cwt/sigslot.hpp>

CWT_NS_BEGIN

class DLL_API Settings : public has_slots<>
{
	CWT_PIMPL_IMPL(Settings);

public:
	enum Format {
		  UnknownFormat
		, JsonFormat
	};

public:
	Settings();
	bool parse(const String & str, Format format = UnknownFormat);
	bool parseFromFile(const char *path, Format format = UnknownFormat, const char *encoding = "utf8");

	UniType operator [] (const String & path) { return value(path); }
	UniType value   (const String & path, const UniType & defaultValue = UniType());
	bool    boolean (const String & path, bool defaultValue = false);
	long_t  integer (const String & path, long_t defaultValue = long_t(0));
	double  number  (const String & path, double defaultValue = double(0));
	String  string  (const String & path, const String & defaultValue = String());
	Vector<UniType>  array  (const String & path, const Vector<UniType> & defaultValue = Vector<UniType>());

/* slots */
	void setValue (const String & path, bool value);
	void setValue (const String & path, double value);
	void setValue (const String & path, float value);
	void setValue (const String & path, long_t value);
	void setValue (const String & path, ulong_t value);
	void setValue (const String & path, int_t value);
	void setValue (const String & path, uint_t value);
	void setValue (const String & path, const String & s);
	void setValue (const String & path, const Char * chars);
	void setValue (const String & path, const Char * chars, size_t size);
	void setValue (const String & path, const char * utf8);
	void setValue (const String & path, const char * utf8, size_t size);
};

CWT_NS_END

#endif /* __CWT_SETTINGS_HPP__ */

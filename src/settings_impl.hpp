/*
 * settings_impl.hpp
 *
 *  Created on: Oct 31, 2013
 *      Author: wladt
 */

#include <cwt/settings.hpp>
#include <cwt/unitype.hpp>
#include <cwt/string.hpp>
#include <cwt/vector.hpp>

CWT_NS_BEGIN

class SettingsImpl
{
public:
	SettingsImpl() {}

	bool    (* parse)   (SettingsImpl * self, const String & str);
	UniType (* value)   (const SettingsImpl * self, const String & path, const UniType & defaultValue);
	bool    (* boolean) (const SettingsImpl * self, const String & path, bool defaultValue);
	long_t  (* integer) (const SettingsImpl * self, const String & path, long_t defaultValue);
	double  (* number)  (const SettingsImpl * self, const String & path, double defaultValue);
	String  (* string)  (const SettingsImpl * self, const String & path, const String & defaultValue);
	Vector<UniType>  (* array)  (const SettingsImpl * self, const String & path, const Vector<UniType> & defaultValue);

	bool (* contains)   (const SettingsImpl * self, const String & path);
	UniType::TypeEnum (* type) (const SettingsImpl * self, const String & path);

	void (* setBool)   (SettingsImpl * self, const String & path, bool value);
	void (* setDouble) (SettingsImpl * self, const String & path, double value);
	void (* setFloat)  (SettingsImpl * self, const String & path, float value);
	void (* setLong)   (SettingsImpl * self, const String & path, long_t value);
	void (* setULong)  (SettingsImpl * self, const String & path, ulong_t value);
	void (* setInt)    (SettingsImpl * self, const String & path, int_t value);
	void (* setUInt)   (SettingsImpl * self, const String & path, uint_t value);
	void (* setString) (SettingsImpl * self, const String & path, const String & s);
	void (* setLatin1) (SettingsImpl * self, const String & path, const char * latin1);

	void * _data;
};

CWT_NS_END



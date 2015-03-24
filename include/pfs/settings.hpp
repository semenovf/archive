/*
 * settings.hpp
 *
 *  Created on: Jun 4, 2013
 *      Author: wladt
 */

#ifndef __PFS_SETTINGS_HPP__
#define __PFS_SETTINGS_HPP__

#include <pfs/pimpl.hpp>
#include <pfs/string.hpp>
#include <pfs/variant.hpp>
#include <pfs/vector.hpp>
#include <pfs/sigslot.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

class settings_impl;

// MT-safe
class DLL_API settings : public nullable<settings_impl>, public has_slots<>
{
protected:
	typedef nullable<settings_impl> base_class;
	typedef settings                self_class;
	typedef settings_impl           impl_class;

public:
	enum format {
		  JsonFormat
		, DefaultFormat = JsonFormat
	};

	class property
	{
		typedef variant<bool, integral_t, real_t, string> value_type;
	private:
		value_type _val;
	public:
		property () : _val(integral_t(0)) {}
		property (bool v) : _val(v) {}
	};

public:
	settings (format f = DefaultFormat);
	~settings ();

	bool parse (const string & str);

	property
	operator [] (const string & path)
	{
		return value(path);
	}

	property
	value (const string & path
			, const property & defaultValue = property()) const;

	bool
	booleanValue (const string & path
			, bool defaultValue = false) const;

	integral_t
	integralValue (const string & path
			, integral_t defaultValue = integral_t(0)) const;

	real_t
	realValue (const string & path
			, real_t defaultValue = real_t(.0f)) const;

	string
	stringValue (const string & path
			, const string & defaultValue = string()) const;

	vector<property>
	arrayValue (const string & path
			, const vector<property> & defaultValue = vector<property>()) const;

	bool contains (const string & path) const;

/* slots */
	void set (const string & path, bool value);
	void set (const string & path, integral_t value);
	void set (const string & path, uintegral_t value);
	void set (const string & path, real_t value);
	void set (const string & path, const string & s);
};

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_SETTINGS_HPP__ */

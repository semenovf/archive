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

// MT-safe
class DLL_API settings : public has_slots<>
{
	pimpl _d;

public:
	// FIXME
	class property
	{
//		typedef variant<bool, integral_t, real_t, string> value_type;
//	private:
//		value_type _val;
//	public:
//		property () : _val(integral_t(0)) {}
//		property (bool v) : _val(v) {}
	};

public:
	settings (const string & path);
	~settings ();

	bool parse (const string & str);

//	property
//	operator [] (const string & path)
//	{
//		return value(path);
//	}
//
//	property
//	value (const string & path
//			, const property & defaultValue = property()) const;
//
	bool
	boolean (const string & path
			, bool defaultValue = false) const;

	integral_t
	integer (const string & path
			, integral_t defaultValue = integral_t(0)) const;

	real_t
	number (const string & path
			, real_t defaultValue = real_t(.0f)) const;

	string
	str (const string & path
			, const string & defaultValue = string()) const;

//	vector<property>
//	arrayValue (const string & path
//			, const vector<property> & defaultValue = vector<property>()) const;

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

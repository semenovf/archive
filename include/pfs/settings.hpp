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
		typedef pfs::variant<bool, integral_t, real_t, pfs::string> value_type;
	private:
		value_type _val;
	public:
		property () : _val(integral_t(0)) {}
		property (bool v) : _val(v) {}
	};

public:
	settings (format f = DefaultFormat);
	~settings ();

	bool parse (const pfs::string & str);

	property
	operator [] (const pfs::string & path)
	{
		return value(path);
	}

	property
	value (const pfs::string & path
			, const property & defaultValue = property()) const;

	bool
	booleanValue (const pfs::string & path
			, bool defaultValue = false) const;

	integral_t
	integralValue (const pfs::string & path
			, integral_t defaultValue = integral_t(0)) const;

	real_t
	realValue (const pfs::string & path
			, real_t defaultValue = real_t(.0f)) const;

	pfs::string
	stringValue (const pfs::string & path
			, const pfs::string & defaultValue = pfs::string()) const;

	pfs::vector<property>
	arrayValue (const pfs::string & path
			, const pfs::vector<property> & defaultValue = pfs::vector<property>()) const;

	bool contains (const pfs::string & path) const;

/* slots */
	void set (const pfs::string & path, bool value);
	void set (const pfs::string & path, integral_t value);
	void set (const pfs::string & path, uintegral_t value);
	void set (const pfs::string & path, real_t value);
	void set (const pfs::string & path, const pfs::string & s);
};

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_SETTINGS_HPP__ */

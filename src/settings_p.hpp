/*
 * settings_impl.hpp
 *
 *  Created on: Oct 31, 2013
 *      Author: wladt
 */

#include "pfs/settings.hpp"

#ifdef __COMMENT__
namespace pfs {

class settings_adapter
{
//public:
//	void * _data;
//
public:
//	settings_adapter () : _data(nullptr) {}
	bool
	(* parse)
		(settings_adapter * self
		, const pfs::string & str);

//	settings::property
//	(* value)
//		(const settings_adapter * self
//		, const pfs::string & path
//		, const settings::property & defaultValue);

	bool
	(* boolean)
		(const settings_adapter * self
		, const pfs::string & path
		, bool defaultValue);

	integral_t
	(* integer)
		(const settings_adapter * self
		, const pfs::string & path
		, integral_t defaultValue);

	real_t
	(* number)
		(const settings_adapter * self
		, const pfs::string & path
		, real_t defaultValue);

	pfs::string
	(* str)
		(const settings_adapter * self
		, const pfs::string & path
		, const pfs::string & defaultValue);

//	pfs::vector<settings::property>
//	(* arrayValue)
//		(const settings_adapter * self
//		, const pfs::string & path
//		, const pfs::vector<settings::property> & defaultValue);

	bool
	(* contains)
		(const settings_adapter * self
		, const pfs::string & path);

	void
	(* setBoolean)
		(settings_adapter * self
		, const pfs::string & path
		, bool value);

	void
	(* setInteger)
		(settings_adapter * self
		, const pfs::string & path
		, integral_t value);

	void
	(* setNumber)
		(settings_adapter * self
		, const pfs::string & path
		, real_t value);

	void
	(* setString)
		(settings_adapter * self
		, const pfs::string & path
		, const pfs::string & s);
};

//class settings_impl : public settings_adapter
//{
//	settings::format _format;
//
//public:
//	settings_impl ()
//		: settings_adapter()
//		, _format(settings::DefaultFormat) {}
//
//	settings_impl (settings::format f)
//		: settings_adapter()
//		, _format(f) {}
//
//	settings::format format () const { return _format; }
//};

} // pfs

#endif

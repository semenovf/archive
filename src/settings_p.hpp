/*
 * settings_impl.hpp
 *
 *  Created on: Oct 31, 2013
 *      Author: wladt
 */

#include "pfs/settings.hpp"

namespace pfs {

class settings_adapter
{
public:
	void * _data;

public:
	settings_adapter () : _data(nullptr) {}

	bool
	(* parse)
		(settings_adapter * self
		, const pfs::string & str);

	settings::property
	(* value)
		(const settings_adapter * self
		, const pfs::string & path
		, const settings::property & defaultValue);

	bool
	(* booleanValue)
		(const settings_adapter * self
		, const pfs::string & path
		, bool defaultValue);

	integral_t
	(* integralValue)
		(const settings_adapter * self
		, const pfs::string & path
		, integral_t defaultValue);

	real_t
	(* realValue)
		(const settings_adapter * self
		, const pfs::string & path
		, real_t defaultValue);

	pfs::string
	(* stringValue)
		(const settings_adapter * self
		, const pfs::string & path
		, const pfs::string & defaultValue);

	pfs::vector<settings::property>
	(* arrayValue)
		(const settings_adapter * self
		, const pfs::string & path
		, const pfs::vector<settings::property> & defaultValue);

	bool
	(* contains)
		(const settings_adapter * self
		, const pfs::string & path);

	void
	(* setBool)
		(settings_adapter * self
		, const pfs::string & path
		, bool value);

	void
	(* setIntegral)
		(settings_adapter * self
		, const pfs::string & path
		, integral_t value);

	void
	(* setUIntegral)
		(settings_adapter * self
		, const pfs::string & path
		, uintegral_t value);

	void
	(* setReal)
		(settings_adapter * self
		, const pfs::string & path
		, real_t value);

	void
	(* setString)
		(settings_adapter * self
		, const pfs::string & path
		, const pfs::string & s);
};

} // pfs

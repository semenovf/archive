#ifndef __API_HPP__
#define __API_HPP__

#define API_QUIT                    1
#define API_CRITICAL                2
#define API_INFO                    3
#define API_DEBUG                   4
#define API_WARN                    5
#define API_ERROR                   6
#define API_GUI_READY               7
#define API_GET_PREF_BOOLEAN        8
#define API_GET_PREF_INTEGER        9
#define API_GET_PREF_NUMBER        10
#define API_GET_PREF_STRING        11

#ifdef INCLUDE_SEPALOID_MAPPING

#include <pfs/string.hpp>
#include <pfs/sepaloid.hpp>
#include <pfs/critical.hpp>

static pfs::sepaloid::mapping_type API[] = {

	  { API_QUIT
			, new pfs::sigslot_mapping0_t
			, _u8("Quit application") }

	, { API_CRITICAL
			, new pfs::sigslot_mapping1_t<const pfs::critical &>
			, _u8("Runtime error") }

	, { API_INFO
			, new pfs::sigslot_mapping1_t<const pfs::string &>
			, _u8("Runtime info message") }

	, { API_DEBUG
			, new pfs::sigslot_mapping1_t<const pfs::string &>
			, _u8("Runtime debug message") }

	, { API_WARN
			, new pfs::sigslot_mapping1_t<const pfs::string &>
			, _u8("Runtime warning message") }

	, { API_ERROR
			, new pfs::sigslot_mapping1_t<const pfs::string &>
			, _u8("Runtime error message") }

	, { API_GUI_READY
			, new pfs::sigslot_mapping1_t<bool>
			, _u8("User interface ready (or not) to interact") }

	, { API_GET_PREF_BOOLEAN
			, new pfs::sigslot_mapping2_t<const pfs::string &, bool *>
			, _u8("Get preference's boolean value") }

	, { API_GET_PREF_INTEGER
			, new pfs::sigslot_mapping2_t<const pfs::string &, integral_t *>
			, _u8("Get preference's integer value") }

	, { API_GET_PREF_NUMBER
			, new pfs::sigslot_mapping2_t<const pfs::string &, real_t *>
			, _u8("Get preference's number value") }

	, { API_GET_PREF_STRING
			, new pfs::sigslot_mapping2_t<const pfs::string &, pfs::string *>
			, _u8("Get preference's string value") }
};

#endif // INCLUDE_SEPALOID_MAPPING

#endif /* __API_HPP__ */

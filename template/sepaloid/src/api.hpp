#ifndef __API_HPP__
#define __API_HPP__

#define API_QUIT                    1
#define API_CRITICAL                2
#define API_INFO                    3
#define API_DEBUG                   4
#define API_WARN                    5
#define API_ERROR                   6
#define API_GUI_READY               7
#define API_GET_PREF                8

#ifdef _INCLUDE_SEPALOID_MAPPING

#include <pfs/string.hpp>
#include <pfs/unitype.hpp>
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

	, { API_GET_PREF
			, new pfs::sigslot_mapping2_t<const pfs::string &, pfs::unitype &>
			, _u8("Get preference value") }

};

#endif // _INCLUDE_SEPALOID_MAPPING

#endif /* __API_HPP__ */

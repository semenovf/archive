/*
 * option.cpp
 *
 *  Created on: Jun 4, 2013
 *      Author: wladt
 */

#include "../include/cwt/option.hpp"
#include <pfs/map.hpp>
#include <cwt/logger.hpp>

namespace cwt {

static const pfs::string __modes_data[][4] = {
	  {_u8("-"), _u8("--"), _u8("="), _u8("\"'") } // Unix
	, {_u8("/"), _u8("/"), _u8(":"), _u8("\"'") } // Windows
};

void OptionsContext::setMode(option::mode_type mode)
{
	_shortPrefix     = __modes_data[mode][0];
	_longPrefix      = __modes_data[mode][1];
	_optArgSeparator = __modes_data[mode][2];
	_quoteChars      = __modes_data[mode][3];
}


static void __split_long_arg (const pfs::string & arg
		, const pfs::string & separator
		, const pfs::string & quotes
		, pfs::string & optname
		, pfs::string & optval)
{
	pfs::string::const_iterator it = arg.find(separator, arg.cbegin());
	if (it == arg.cend()) {
		optname = arg;
	} else {
		optname = arg.substr(arg.cbegin(), it);
		optval  = arg.substr(++it);
		if (!optval.isEmpty() && quotes.find(pfs::string(1, optval[0])) != quotes.cend()) { // optval starts with one of the quote char
			if (optval.size() > 1) {
				optval = optval.substr(1, optval.size() - 2);
			} else {
				cwt::error(_Fr("%s: bad option?") % arg);
				optval.clear();
			}
		}
	}
}

bool OptionsContext::parse_opts(cwt::settings & settings
		, int argc
		, char * argv[]
		, size_t optc
		, const option optv[]
		, pfs::vector<pfs::string> * args)
{
	if (argc <= 0)
		return true;

	bool r = true;
	pfs::map<pfs::string, const option *> optmap;

	for (size_t i = 0; i < optc; ++i) {
		if (!optv[i].shortname.isEmpty()) {
			if(optmap.contains(optv[i].shortname)) {
				cwt::error(_Fr("%s: duplication of short option") % optv[i].shortname);
				return false;
			}
			optmap.insert(optv[i].shortname, & optv[i]);
		}

		if (!optv[i].longname.isEmpty()) {
			if(optmap.contains(optv[i].longname)) {
				cwt::error(_Fr("%s: duplication of long option") % optv[i].longname);
				return false;
			}
			optmap.insert(optv[i].longname, & optv[i]);
		}
	}

	for (int i = 1; i < argc; ++i) {
#if defined(CWT_OS_WIN32) || defined(CWT_OS_WIN64)
		pfs::string arg = pfs::string::fromUtf16(argv[i]);
#else
		pfs::string arg = pfs::string::fromUtf8(argv[i]);
#endif
		pfs::string optname;
		pfs::string optval;
		bool skip = false;

		if (arg.startsWith(_longPrefix)) {
			__split_long_arg(arg.substr(_longPrefix.length()), _optArgSeparator, _quoteChars, optname, optval);
		} else if (arg.startsWith(_shortPrefix)) {
			optname = arg.substr(_shortPrefix.length());
			if (i + 1 < argc) {
				optval = _u8(argv[i + 1]);
				skip = true;
			}
		} else { // argument
			if (args)
				args->append(arg);
			continue;
		}

		if (optname.isEmpty() || !optmap.contains(optname)) {
			cwt::warn(_Fr("%s: bad option") % optname);
			continue;
		}

		const option * opt = optmap[optname];

		if (opt->has_arg) {
			if (optval.isEmpty()) {
				cwt::error(_Fr("%s: option need an argument") % optname);
				r = false;
				break;
			} else {
				settings.set(opt->xpath, optval);
				if (skip) // skip argument for short option
					++i;
			}
		} else {
			settings.set(opt->xpath, true);
		}
	}

	return r;
}

} // cwt

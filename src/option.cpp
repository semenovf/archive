/*
 * option.cpp
 *
 *  Created on: Jun 4, 2013
 *      Author: wladt
 */

#include "pfs/option.hpp"
#include "pfs/map.hpp"
#include "pfs/logger.hpp"

namespace pfs {

static const string __modes_data[][4] = {
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


static void __split_long_arg (const string & arg
		, const string & separator
		, const string & quotes
		, string & optname
		, string & optval)
{
	string::const_iterator it = arg.find(arg.cbegin(), separator);
	if (it == arg.cend()) {
		optname = arg;
	} else {
		optname = arg.substr(arg.cbegin(), it);
		++it;
		optval  = arg.substr(it);
		if (!optval.isEmpty() && quotes.find(string(1, optval[0])) != quotes.cend()) { // optval starts with one of the quote char
			if (optval.size() > 1) {
				optval = optval.substr(1, optval.size() - 2);
			} else {
				string m(arg);
				error(m << ": " << _Tr("bad option?"));
				optval.clear();
			}
		}
	}
}

bool OptionsContext::parse_opts(settings & settings
		, int argc
		, char * argv[]
		, size_t optc
		, const option optv[]
		, vector<string> * args)
{
	if (argc <= 0)
		return true;

	bool r = true;
	map<string, const option *> optmap;

	for (size_t i = 0; i < optc; ++i) {
		if (!optv[i].shortname.isEmpty()) {
			if(optmap.contains(optv[i].shortname)) {
				string m(optv[i].shortname);
				error(m << ": " << _Tr("duplication of short option"));
				return false;
			}
			optmap.insert(optv[i].shortname, & optv[i]);
		}

		if (!optv[i].longname.isEmpty()) {
			if(optmap.contains(optv[i].longname)) {
				string m(optv[i].longname);
				error(m << ": " << _Tr("duplication of long option"));
				return false;
			}
			optmap.insert(optv[i].longname, & optv[i]);
		}
	}

	for (int i = 1; i < argc; ++i) {
#if defined(PFS_OS_WIN32) || defined(PFS_OS_WIN64)
		string arg = string::fromUtf16(argv[i]);
#else
		string arg = string::fromUtf8(argv[i]);
#endif
		string optname;
		string optval;
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
			string m(optname);
			m << ": " << _Tr("bad option");
			warn(m);
			continue;
		}

		const option * opt = optmap[optname];

		if (opt->has_arg) {
			if (optval.isEmpty()) {
				string m(optname);
				error(m << ": " << _Tr("option need an argument"));
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

} // pfs

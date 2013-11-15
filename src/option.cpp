/*
 * option.cpp
 *
 *  Created on: Jun 4, 2013
 *      Author: wladt
 */

#include "../include/cwt/option.hpp"
#include <cwt/map.hpp>
#include <cwt/logger.hpp>

CWT_NS_BEGIN

static const String __modes_data[][4] = {
	  {"-", "--", "=", "\"'" } // Unix
	, {"/", "/" , ":", "\"'" } // Windows
};

void OptionsContext::setMode(Option::Mode mode)
{
	_shortPrefix     = __modes_data[mode][0];
	_longPrefix      = __modes_data[mode][1];
	_optArgSeparator = __modes_data[mode][2];
	_quoteChars      = __modes_data[mode][3];
}


static void __split_long_arg(const String & arg
		, const String & separator
		, const String & quotes
		, String & optname
		, String & optval)
{
	String::const_iterator it = arg.find(separator, arg.cbegin());
	if (it == arg.cend()) {
		optname = arg;
	} else {
		optname = arg.substr(arg.cbegin(), it);
		optval  = arg.substr(++it);
		if (!optval.isEmpty() && quotes.find(String(1, optval[0])) != quotes.cend()) { // optval starts with one of the quote char
			if (optval.size() > 1) {
				optval = optval.substr(1, optval.size() - 2);
			} else {
				Logger::error(_Fr("%s: bad option?") % arg);
				optval.clear();
			}
		}
	}
}

bool OptionsContext::parse_opts(Settings & settings
		, int argc
		, char * argv[]
		, size_t optc
		, const Option optv[]
		, Vector<String> * args)
{
	if (argc <= 0)
		return true;

	bool r = true;
	Map<String, const Option *> optmap;

	for (size_t i = 0; i < optc; ++i) {
		if (!optv[i].shortname.isEmpty()) {
			if(optmap.contains(optv[i].shortname)) {
				Logger::error(_Fr("%s: duplication of short option") % optv[i].shortname);
				return false;
			}
			optmap.insert(optv[i].shortname, & optv[i]);
		}

		if (!optv[i].longname.isEmpty()) {
			if(optmap.contains(optv[i].longname)) {
				Logger::error(_Fr("%s: duplication of long option") % optv[i].longname);
				return false;
			}
			optmap.insert(optv[i].longname, & optv[i]);
		}
	}

	for (int i = 1; i < argc; ++i) {
#if defined(CWT_OS_WIN32) || defined(CWT_OS_WIN64)
		String arg = String::fromUtf16(argv[i]);
#else
		String arg = String::fromUtf8(argv[i]);
#endif
		String optname;
		String optval;
		bool skip = false;

		if (arg.startsWith(_longPrefix)) {
			__split_long_arg(arg.substr(_longPrefix.length()), _optArgSeparator, _quoteChars, optname, optval);
		} else if (arg.startsWith(_shortPrefix)) {
			optname = arg.substr(_shortPrefix.length());
			if (i + 1 < argc) {
				optval = argv[i + 1];
				skip = true;
			}
		} else { // argument
			if (args)
				args->append(arg);
			continue;
		}

		if (optname.isEmpty() || !optmap.contains(optname)) {
			Logger::warn(_Fr("%s: bad option") % optname);
			continue;
		}

		const Option * opt = optmap[optname];

		if (opt->has_arg) {
			if (optval.isEmpty()) {
				Logger::error(_Fr("%s: option need an argument") % optname);
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


CWT_NS_END

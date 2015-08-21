/*
 * cli.cpp
 *
 *  Created on: Aug 20, 2015
 *      Author: wladt
 */

#include "pfs/cli.hpp"

namespace pfs { namespace cli {

router & cli::r ()
{
	_routers.append(router());
	return _routers.refAt(_routers.size() - 1);
}

#ifdef __COMMENT__

command_option & command_line::getOption (const stringlist & optnames)
{
    stringlist::const_iterator itEnd = optnames.cend();
    for (stringlist::const_iterator it = optnames.cbegin()
            ; it != itEnd; ++it) {
        if (_options.contains(*it)) {
            addError(*it + _u8(": option already exists"));
            return _dummyOption;
        }
    }
}

command_option & command_line::booleanOption (const stringlist & optnames)
{
}

command_option & command_line::integerOption (const stringlist & optnames)
{}

command_option & command_line::numberOption (const stringlist & optnames)
{}

command_option & command_line::stringOption (const stringlist & optnames)
{}

#endif

//} // pfs::cli

#ifdef __COMMENT__


static const string modes_data[][4] = {
	  {_l1("-"), _l1("--"), _l1("="), _l1("\"'") } // Unix
	, {_l1("/"), _l1("/"), _l1(":"), _l1("\"'") } // Windows
};


//
// Do not want to include <wchar.h>
//
static size_t argvlen (const argv_t * p)
{
	size_t r = 0;
	while (*p != 0) ++r;
	return r;
}

static string argv_to_string (const argv_t * arg)
{
	switch (sizeof(argv_t)) {
	case 1: // char
#ifdef PFS_UNICODE
		return string::fromUtf8(arg);
#else
		// TODO Need to implement string::fromLocal8Bit()/toLocal8Bit()
#		error "Need to implement string::fromLocal8Bit()/toLocal8Bit()"
#endif

	case 2:  // Microsoft wchar_t
		return string::fromUtf16(reinterpret_cast<const uint16_t*>(arg), argvlen(arg));
		break;

	case 4:  // Unix wchar_t
	default:
		// TODO Need to implement mbcs_string::fromUtf32()
		//return string::fromUtf32(reinterpret_cast<const uint32_t*>(arg), argvlen(arg));
		PFS_ASSERT_UNEXPECTED();
		break;
	}
	return string();
}


static void split_long_arg (const string & arg
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

bool command_line::parseOpts (settings & s
		, int argc
		, argv_t * argv[]
		, size_t optc
		, const option optv[]
		, stringlist * args)
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
		string arg = argv_to_string(argv[i]);
		string optname;
		string optval;
		bool skip = false;

		if (arg.startsWith(_longPrefix)) {
			split_long_arg(arg.substr(_longPrefix.length()), _optArgSeparator, _quoteChars, optname, optval);
		} else if (arg.startsWith(_shortPrefix)) {
			optname = arg.substr(_shortPrefix.length());

			if (i + 1 < argc) {
				optval = argv_to_string(argv[i + 1]);
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
				s.setString(opt->xpath, optval);
				if (skip) // skip argument for short option
					++i;
			}
		} else {
			s.setBoolean(opt->xpath, true);
		}
	}

	return r;
}
#endif
} } // pfs::cli

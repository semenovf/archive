/*
 * options.hpp
 *
 *  Created on: Jun 4, 2013
 *      Author: wladt
 */

#ifndef __CWT_OPTIONS_HPP__
#define __CWT_OPTIONS_HPP__

#include <pfs/vector.hpp>
#include <pfs/string.hpp>
#include <cwt/settings.hpp>

namespace cwt {

struct option
{
	enum mode_type {
		  Unix
		, Windows
	};

	pfs::string longname;     // long option or NULL
	pfs::string shortname;    // short option or 0
	bool        has_arg;      // true if option has argument
	pfs::string xpath;        // path to appropriate settings node
	pfs::string defvalue;     // default value
	pfs::string desc;         // option's description (for automatically generated usage)
//	bool        (*validator)(const void*);  /* validation function for option argument */
};

class DLL_API OptionsContext
{
public:
	OptionsContext() {
	// TODO Need more flexible approach to distinguish OS specific
	// features (may by using global repository based on some DOM instance)
#if defined(PFS_OS_WIN32) || defined(PFS_OS_WIN64)
		setMode (option::Windows);
#else
		setMode (option::Unix);
#endif
	}

	void setShortPrefix (const pfs::string & prefix) { _shortPrefix = prefix; }
	void setLongPrefix  (const pfs::string & prefix)  { _longPrefix = prefix; }
	void setOptArgsSeparator(const pfs::string & separator) { _optArgSeparator = separator; }
	void setQuoteChars  (const pfs::string & quotes)  { _quoteChars = quotes; }
	void setMode        (option::mode_type mode);

	bool parse (cwt::settings & settings, int argc, char * argv[], size_t optc, const option optv[], pfs::vector<pfs::string> & args)
		{ return parse_opts(settings, argc, argv, optc, optv, & args); }
	bool parse (cwt::settings & settings, int argc, char * argv[], size_t optc, const option optv[])
		{ return parse_opts(settings, argc, argv, optc, optv, nullptr); }

private:
	bool parse_opts(cwt::settings & settings, int argc, char * argv[], size_t optc, const option optv[], pfs::vector<pfs::string> * args);

private:
	pfs::string   _shortPrefix;
	pfs::string   _longPrefix;
	pfs::string   _optArgSeparator;
	pfs::string   _quoteChars;
};

} // cwt

#endif /* __CWT_OPTIONS_HPP__ */

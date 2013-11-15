/*
 * options.hpp
 *
 *  Created on: Jun 4, 2013
 *      Author: wladt
 */

#ifndef __CWT_OPTIONS_HPP__
#define __CWT_OPTIONS_HPP__

#include <cwt/vector.hpp>
#include <cwt/settings.hpp>

CWT_NS_BEGIN

struct Option
{
	enum Mode {
		  Unix
		, Windows
	};

	String       longname;     // long option or NULL
	String       shortname;    // short option or 0
	bool         has_arg;      // true if option has argument
	String       xpath;        // path to appropriate settings node
	String       defvalue;     // default value
	String       desc;         // option's description (for automatically generated usage)
//	bool        (*validator)(const void*);  /* validation function for option argument */
};

class DLL_API OptionsContext
{
public:
	OptionsContext() {
	// TODO Need more flexible approach to distinguish OS specific features (may by using global repository based on some DOM instance)
#if defined(CWT_OS_WIN32) || defined(CWT_OS_WIN64)
		setMode (Option::Windows);
#else
		setMode (Option::Unix);
#endif
	}

	void setShortPrefix(const String & prefix) { _shortPrefix = prefix; }
	void setLongPrefix(const String & prefix)  { _longPrefix = prefix; }
	void setOptArgsSeparator(const String & separator) { _optArgSeparator = separator; }
	void setQuoteChars(const String & quotes)  { _quoteChars = quotes; }
	void setMode(Option::Mode mode);

	bool parse(Settings & settings, int argc, char * argv[], size_t optc, const Option optv[], Vector<String> & args)
		{ return parse_opts(settings, argc, argv, optc, optv, & args); }
	bool parse(Settings & settings, int argc, char * argv[], size_t optc, const Option optv[])
		{ return parse_opts(settings, argc, argv, optc, optv, nullptr); }

private:
	bool parse_opts(Settings & settings, int argc, char * argv[], size_t optc, const Option optv[], Vector<String> * args);

private:
	String   _shortPrefix;
	String   _longPrefix;
	String   _optArgSeparator;
	String   _quoteChars;
};

CWT_NS_END

#endif /* __CWT_OPTIONS_HPP__ */

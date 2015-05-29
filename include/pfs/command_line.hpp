/*
 * command_line.hpp
 *
 *  Created on: Jun 4, 2013
 *      Author: wladt
 */

#ifndef __PFS_OPTIONS_HPP__
#define __PFS_OPTIONS_HPP__

#include <pfs/command_line.hpp>
#include <pfs/string.hpp>
#include <pfs/stringlist.hpp>
#include <pfs/settings.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

struct option
{
	enum mode_type {
		  Unix
		, Windows
	};

	string longname;     // long option or NULL
	string shortname;    // short option or 0
	bool   has_arg;      // true if option has argument
	string xpath;        // path to appropriate settings node
	string defvalue;     // default value
	string desc;         // option's description (for automatically generated usage)
//	bool        (*validator)(const void*);  /* validation function for option argument */
};

class DLL_API command_line
{
    string _shortPrefix;
    string _longPrefix;
    string _optArgSeparator;
    string _quoteChars;

public:
	command_line ();

	void setShortPrefix (const string & prefix) { _shortPrefix = prefix; }
	void setLongPrefix  (const string & prefix)  { _longPrefix = prefix; }
	void setOptArgsSeparator(const string & separator) { _optArgSeparator = separator; }
	void setQuoteChars  (const string & quotes)  { _quoteChars = quotes; }
	void setMode        (option::mode_type mode);

	bool parse (settings & s, int argc, argv_t * argv[]
			, size_t optc, const option optv[], stringlist & args)
	{
	    return parseOpts(s, argc, argv, optc, optv, & args);
	}

	bool parse (settings & s, int argc, argv_t * argv[], size_t optc, const option optv[])
	{
	    return parseOpts(s, argc, argv, optc, optv, nullptr);
	}

private:
	bool parseOpts (settings & s
			, int argc
			, argv_t * argv[]
	        , size_t optc
	        , const option optv[]
	        , stringlist * args);
};

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_OPTIONS_HPP__ */

/*
 * command_line.hpp
 *
 *  Created on: Jun 4, 2013
 *      Author: wladt
 */

#ifndef __PFS_OPTIONS_HPP__
#define __PFS_OPTIONS_HPP__

#include <pfs/command_line.hpp>
#include <pfs/shared_ptr.hpp>
#include <pfs/map.hpp>
#include <pfs/string.hpp>
#include <pfs/stringlist.hpp>
#include <pfs/settings.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

namespace option_format {
	enum {
		  DashPrefix       = 0x0001
		, DoubleDashPrefix = 0x0002
		, SlashPrefix      = 0x0004
		, AssignSeparator  = 0x0008
		, ColonSeparator   = 0x0010
		, SpaceSeparator   = 0x0020
	};

	static const int UnixLongOption     = DoubleDashPrefix | AssignSeparator;
	static const int UnixShortOption    = DashPrefix | SpaceSeparator;
	static const int WindowsLongOption  = SlashPrefix | SpaceSeparator;
	static const int WindowsShortOption = SlashPrefix | ColonSeparator;
	static const int Any =
			  DashPrefix
			| DoubleDashPrefix
			| SlashPrefix
			| AssignSeparator
			| ColonSeparator
			| SpaceSeparator;
	static const int None = 0;
}

struct option
{
//	string longname;     // long option or NULL
//	string shortname;    // short option or 0
//	bool   has_arg;      // true if option has argument
//	string xpath;        // path to appropriate settings node
//	string defvalue;     // default value
//	string desc;         // option's description (for automatically generated usage)
////	bool        (*validator)(const void*);  /* validation function for option argument */
};

class DLL_API command_line
{
	int _shortoptFormat;
	int _longoptFormat;
	map<string, shared_ptr<option> > _options;

public:
    // Default constructor depends on OS platform
	command_line ();

	/**
	 * @brief Sets option format for appropriate type of option.
	 *
	 * @param shortoptFormat Combination of values defined at option_format namespace
	 *      applied to short option. If value is 0 (option_format::None), so this type
	 * 		of option does not recognized by parser and issues an error.
	 * @param longoptFormat  Combination of values defined at option_format namespace
	 * 		applied to long option. If value is 0 (option_format::None), so this type
	 * 		of option does not recognized by parser and issues an error.
	 */
	void setOptionFormat (int shortoptFormat, int longoptFormat)
	{
		_shortoptFormat = shortoptFormat;
		_longoptFormat  = longoptFormat;
	}

private:
	option & booleanOption (const string & optname)
		{ return booleanOption(stringlist() << optname); }
	option & integerOption (const string & optname)
		{ return integerOption(stringlist() << optname); }
	option & numberOption (const string & optname);
	option & stringOption (const string & optname);

	option & booleanOption (const string & optname1, const string & optname1);
	option & integerOption (const string & optname1, const string & optname1);
	option & numberOption (const string & optname1, const string & optname1);
	option & stringOption (const string & optname1, const string & optname1);

	option & booleanOption (const stringlist & optnames);
	option & integerOption (const stringlist & optnames);
	option & numberOption (const stringlist & optnames);
	option & stringOption (const stringlist & optnames);

//	bool parse (settings & s, int argc, argv_t * argv[]
//			, size_t optc, const option optv[], stringlist * args = nullptr)
//	{
//	    return parseOpts(s, argc, argv, optc, optv, args);
//	}
//
//	bool parse (settings & s, int argc, argv_t * argv[], size_t optc, const option optv[])
//	{
//	    return parseOpts(s, argc, argv, optc, optv, nullptr);
//	}
//
//private:
//	bool parseOpts (settings & s
//			, int argc
//			, argv_t * argv[]
//	        , size_t optc
//	        , const option optv[]
//	        , stringlist * args);
};

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_OPTIONS_HPP__ */

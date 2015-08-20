/*
 * cli.hpp
 *
 *  Created on: Aug 20, 2015
 *      Author: wladt
 */

#ifndef __PFS_CLI_HPP__
#define __PFS_CLI_HPP__

#include <pfs/shared_ptr.hpp>
#include <pfs/map.hpp>
#include <pfs/string.hpp>
#include <pfs/stringlist.hpp>
#include <pfs/errorable.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs { namespace cli {

class cli
{
public:
	cli () {}
	bool parse () { return false; }
};


#ifdef __COMMENT__

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


template <typename T>
class DLL_API command_option
{
public:
    struct validator
    {
        bool operator () (const T &);
    };

    struct settings_handler
    {
        bool operator () (settings & s, const string & key) { s.}
    };

protected:
	bool   _hasArg;       // true if option has argument
	string _description;  // option description (for automatically generated usage)
	T      _defaultValue;
	vector<bool (*) ()>  _validators;

public:
	command_option () : _hasArg () {}

	command_option & defaultValue (const T & v)
	{
	    _defaultValue = v;
	    return *this;
	}

	command_option & validate (bool (* validator) (const T & v))
	{
	    _validators.append(validator);
	    return *this;
	}

	command_option & handle (settings & s, const string & key);
	command_option & handle (settings & s, const string & key);
};

class DLL_API command_line : public errorable
{
	int _shortoptFormat;
	int _longoptFormat;
//	command_option _dummyOption;
	map<string, shared_ptr<command_option> > _options;

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

	template <typename T>
	command_option<T> & addOption (const char * optname)
	    { return addOption<T>(string::fromLatin1(optname)); }

	template <typename T>
	command_option & addOption (const string & optname)
		{ return addOption<T>(stringlist() << optname); }

	template <typename T>
    command_option<T> & addOption (const char * optname1, const char * optname2)
        { return addOption<T>(string::fromLatin1(optname1), string::fromLatin1(optname1)); }

	template <typename T>
	command_option<T> & addOption (const string & optname1, const string & optname2)
	    { return addOption<T>(stringlist() << optname1 << optname2); }

	template <typename T>
	command_option<T> & addOption (const stringlist & optnames);

private:

//    command_option & getOption (const stringlist & optnames);

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

template <typename T>
command_option<T> & command_line::addOption (const stringlist & optnames)
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

#endif // __COMMENT__

} } // pfs::cli

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_CLI_HPP__ */

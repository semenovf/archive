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
#include <pfs/vector.hpp>
#include <pfs/string.hpp>
#include <pfs/stringlist.hpp>
#include <pfs/errorable.hpp>
//#include <pfs/variant.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs { namespace cli {

class DLL_API router
{
	struct optdata
	{
		// > 0 - index of default value or index of value after parsing
		//  -1 - if there is no default value or specified option did not found
		int index;
		int first;
		int count;
	};

//	typedef variant<bool, integral_t, real_t, string> variant_type;
	typedef vector<vector<string> > action_list_type;
	typedef map<string,optdata>     option_list_type;
	typedef vector<string>          args_list_type;

	action_list_type _actions;
	option_list_type _options;
	args_list_type   _args;

public:
	static const vector<string> AnyAction;

public:
	router () {}

	router & a (const vector<string> & actions);
	router & a (const string & action);
	router & a (const char * action);
	router & synonym (const string & action);
	router & synonym (const char * action);
	router & syn (const string & action) { return synonym(action); }
	router & syn (const char * action) { return synonym(action); }

	template <typename T>
	router & o (const string & optname);

	template <typename T>
	router & o (const char * optname)
	{
		return o<T>(string::fromLatin1(optname));
	}

	template <typename T>
	router & o (const string & optname, const T & defaultValue);

	template <typename T>
	router & o (const char * optname, const T & defaultValue)
	{
		return o<T>(string::fromLatin1(optname), defaultValue);
	}

	template <typename T>
	router & operator () (const T & validValue);

//	router & b (const string & optname);
//	router & b (const char *   optname);
//	router & b (const string & optname, bool defaultValue);
//	router & b (const char *   optname, bool defaultValue);
//
//	router & i (const string & optname);
//	router & i (const char *   optname);
//	router & i (const string & optname, integral_t defaultValue);
//	router & i (const char *   optname, integral_t defaultValue);
//	router & operator () (integral_t validValue);
//
//	router & n (const string & optname);
//	router & n (const char *   optname);
//	router & n (const string & optname, real_t defaultValue);
//	router & n (const char *   optname, real_t defaultValue);
//	router & operator () (real_t validValue);
//
//	router & s (const string & optname);
//	router & s (const char *   optname);
//	router & s (const string & optname, const string & defaultValue);
//	router & s (const char *   optname, const string & defaultValue);
//	router & operator () (const string & validValue);
};

class DLL_API cli
{
	string _shortOptionPrefix;
	string _longOptionPrefix;
	string _shortOptargSeparator;
	string _longOptargSeparator;
	uint32_t _flags;
	vector<router> _routers;

public:
	enum {
		  AllowShortOption = 0x0001
		, AllowLongOption  = 0x0002
		, CaseSensitive    = 0x0004
	};
public:
	cli ();
	bool parse (int /*argc*/, const char * argv[]) { return false; }

	void allowShortOption (bool b)
	{
		if (b) _flags |= AllowShortOption;
		else   _flags &= ~AllowShortOption;
	}

	void allowLongOption (bool b)
	{
		if (b) _flags |= AllowLongOption;
		else   _flags &= ~AllowLongOption;
	}

	void setCaseSensitive (bool b)
	{
		if (b) _flags |= CaseSensitive;
		else   _flags &= ~CaseSensitive;
	}

	void setShortOptionPrefix (const string & prefix)
	{
		_shortOptionPrefix = prefix;
	}

	void setShortOptionPrefix (const char * prefix)
	{
		setShortOptionPrefix(string::fromLatin1(prefix));
	}

	void setLongOptionPrefix (const string & prefix)
	{
		_longOptionPrefix = prefix;
	}

	void setLongOptionPrefix (const char * prefix)
	{
		setLongOptionPrefix(string::fromLatin1(prefix));
	}

	void setShortOptargSeparator (const string & separator)
	{
		_shortOptargSeparator = separator;
	}

	void setShortOptargSeparator (const char * separator)
	{
		setShortOptargSeparator(string::fromLatin1(separator));
	}

	void setLongOptargSeparator (const string & separator)
	{
		_longOptargSeparator = separator;
	}

	void setLongOptargSeparator (const char * separator)
	{
		setLongOptargSeparator(string::fromLatin1(separator));
	}

	router & r ();
};


template <typename T>
router & router::o (const string & optname)
{
	return *this;
}


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

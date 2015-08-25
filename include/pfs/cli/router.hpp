/*
 * router.hpp
 *
 *  Created on: Aug 25, 2015
 *      Author: wladt
 */

#ifndef __PFS_CLI_ROUTER_HPP__
#define __PFS_CLI_ROUTER_HPP__

#include <pfs/cli/action.hpp>

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

		optdata () : index(-1), first(-1), count(0) {}
	};

//	typedef vector<stringlist>  stringlist_collection_type;
//	typedef map<string,optdata> option_collection_type;

	vector<action> _actions;
//	option_collection_type     _options;
//	stringlist                 _args;
//	optdata                    _activeOptdata;
//	bool                       _booleanValues[2];
//	vector<integral_t>         _integralValues;
//	vector<real_t>             _realValues;
//	stringlist                 _stringValues;

public:
	router () {
//		_booleanValues[0] = false;
//		_booleanValues[1] = true;
	}

//	router & a (const stringlist & actions);
	router & a (const string & action);
	router & a (const char * action);
	router & synonym (const string & action);
	router & synonym (const char * action);
	router & syn (const string & action) { return synonym(action); }
	router & syn (const char * action) { return synonym(action); }

//	router & b (const string & optname);
//	router & b (const char *   optname)
//	{
//		return b(string::fromLatin1(optname));
//	}
//	router & b (const string & optname, bool defaultValue);
//	router & b (const char *   optname, bool defaultValue)
//	{
//		return b(string::fromLatin1(optname), defaultValue);
//	}
//
//	router & i (const string & optname);
//	router & i (const char *   optname)
//	{
//		return i(string::fromLatin1(optname));
//	}
//	router & i (const string & optname, integral_t defaultValue);
//	router & i (const char *   optname, integral_t defaultValue)
//	{
//		return i(string::fromLatin1(optname), defaultValue);
//	}
//	router & operator () (integral_t validValue);

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

}} // pfs::cli

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_CLI_ROUTER_HPP__ */

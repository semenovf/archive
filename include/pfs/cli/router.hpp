/*
 * router.hpp
 *
 *  Created on: Aug 25, 2015
 *      Author: wladt
 */

#ifndef __PFS_CLI_ROUTER_HPP__
#define __PFS_CLI_ROUTER_HPP__

#include <pfs/map.hpp>
#include <pfs/vector.hpp>
#include <pfs/stringlist.hpp>
#include <pfs/cli/action.hpp>
#include <pfs/cli/option.hpp>
//#include <pfs/cli/composer.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs { namespace cli {

class DLL_API router
{
	friend class composer;

	typedef vector<action>      action_collection;
	typedef map<string, option> option_collection;

	action_collection _actions;
	option_collection _options;

private:
	void appendAction (const action & a)
	{
		_actions.append(a);
	}

	void insertOption (const string & optname, const option & opt)
	{
		_options.insert(optname, opt);
	}

	size_t actionCount () const
	{
		return _actions.size();
	}

	action * actionPtr (size_t index)
	{
		return & _actions[index];
	}

	bool containsOption (const string & optname)
	{
		return _options.contains(optname);
	}

	option * optionPtr (const string & optname)
	{
		return & _options.refAt(optname);
	}

public:
	router () {}


//	router & b (const string & optname);
//	router & b (const char *   optname)
//	{
//		return b(string::fromLatin1(optname));
//	}
//
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
//
//	router & i (const string & optname, integral_t defaultValue);
//	router & i (const char *   optname, integral_t defaultValue)
//	{
//		return i(string::fromLatin1(optname), defaultValue);
//	}
////	router & operator () (integral_t validValue);
//
//	router & n (const string & optname);
//	router & n (const char *   optname)
//	{
//		return n(string::fromLatin1(optname));
//	}
//
//	router & n (const string & optname, real_t defaultValue);
//	router & n (const char *   optname, real_t defaultValue)
//	{
//		return n(string::fromLatin1(optname), defaultValue);
//	}
////	router & operator () (real_t validValue);
//
//	router & s (const string & optname);
//	router & s (const char *   optname)
//	{
//		return n(string::fromLatin1(optname));
//	}
//
//	router & s (const string & optname, const string & defaultValue);
//	router & s (const char *   optname, const string & defaultValue)
//	{
//		return s(string::fromLatin1(optname), defaultValue);
//	}
////	router & operator () (const string & validValue);
};

}} // pfs::cli

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_CLI_ROUTER_HPP__ */

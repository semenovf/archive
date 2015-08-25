/*
 * router.hpp
 *
 *  Created on: Aug 25, 2015
 *      Author: wladt
 */

#ifndef __PFS_CLI_ROUTER_HPP__
#define __PFS_CLI_ROUTER_HPP__

#include <pfs/map.hpp>
#include <pfs/stringlist.hpp>
#include <pfs/shared_ptr.hpp>
#include <pfs/cli/action.hpp>
#include <pfs/cli/option.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs { namespace cli {

class DLL_API router
{
	typedef map<string, shared_ptr<option> > option_collection_type;

	vector<action>         _actions;
	option_collection_type _options;
	stringlist             _args;

private:
	template <typename T>
	router & o (const string & optname);

public:
	router () {}

	router & a (const string & name);
	router & a (const char * name);
	router & synonym (const string & name);
	router & synonym (const char * name);
	router & syn (const string & name) { return synonym(name); }
	router & syn (const char * name) { return synonym(name); }

	router & b (const string & optname);
	router & b (const char *   optname)
	{
		return b(string::fromLatin1(optname));
	}
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

/*
 * router.cpp
 *
 *  Created on: Aug 20, 2015
 *      Author: wladt
 */

#include "pfs/cli/router.hpp"

namespace pfs { namespace cli {

//template <typename T>
//router & router::o (const string & optname)
//{
//	option opt = make_option<T>(optname);
//	_options.insert(optname, opt);
//	return *this;
//}
//
//template <typename T>
//router & router::o (const string & optname, const T & defaultValue)
//{
//	option opt = make_option<T>(optname);
//	opt.setDefault(defaultValue);
//	_options.insert(optname, opt);
//	return *this;
//}

//template <typename T>
//router & router::o (const string & optname, const string & description)
//{
//	option opt = make_option<T>(optname);
//	opt.setDescription(description);
//	_options.insert(optname, opt);
//	return *this;
//}

//router & router::b (const string & optname)
//{
//	return o<bool>(optname);
//}
//
//router & router::i (const string & optname)
//{
//	return o<integral_t>(optname);
//}
//
//router & router::n (const string & optname)
//{
//	return o<real_t>(optname);
//}
//
//router & router::s (const string & optname)
//{
//	return o<string>(optname);
//}
//
//router & router::b (const string & optname, bool defaultValue)
//{
//	return o<bool>(optname, defaultValue);
//}
//
//router & router::i (const string & optname, integral_t defaultValue)
//{
//	return o<integral_t>(optname, defaultValue);
//}
//
//router & router::n (const string & optname, real_t defaultValue)
//{
//	return o<real_t>(optname, defaultValue);
//}
//
//router & router::s (const string & optname, const string & defaultValue)
//{
//	return o<string>(optname, defaultValue);
//}

#ifdef __COMMENT__
/**
 *
 * @param optname
 * @param defaultValue
 * @return
 */
router & router::b (const string & optname, bool defaultValue)
{
	_activeOptdata(optdata()); // reset
	_activeOptdata.index = defaultValue ? 1 : 0;
	_options.insert(optname, _activeOptdata);
	return *this;
}

router & router::i (const string & optname)
{
	_activeOptdata(optdata()); // reset
	_options.insert(optname, _activeOptdata);
	return *this;
}

router & router::i (const string & optname, integral_t defaultValue)
{
	_activeOptdata(optdata()); // reset
	_activeOptdata.index = _integralValues.size();
	_integralValues.append(defaultValue);
	_options.insert(optname, _activeOptdata);
	return *this;
}

router & router::operator () (integral_t validValue)
{

	return *this;
}
#endif

}} // pfs::cli


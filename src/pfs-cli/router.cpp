/*
 * router.cpp
 *
 *  Created on: Aug 20, 2015
 *      Author: wladt
 */

#include "pfs/cli/router.hpp"

namespace pfs { namespace cli {

#ifdef __COMMENT__
/**
 * @brief Appends action (first element of vector) and it's alternatives.
 *
 * @param alts Contains action and it's alternatives.
 *        @c alts can be null value means any action (@see AnyAction)
 * @return Reference to router itself.
 *
 */
router & router::a (const stringlist & synonyms)
{
	_actions.append(synonyms);
	return *this;
}
#endif

/**
 * @brief Appends action.
 *
 * @param name
 * 		Action name.
 * @return Reference to router itself.
 */
router & router::a (const string & name)
{
	if (name.isEmpty())
		_actions.append(action::any());
	else
		_actions.append(action(name));
	return *this;
}

router & router::a (const char * name)
{
	if (!name)
		return a(string());
	return a(string::fromLatin1(name));
}

/**
 * @brief Adds synonym for action.
 *
 * @param action Synonym for action.
 * @return Reference to router itself.
 *
 * @see router::synonym(const char *)
 */
router & router::synonym (const string & name)
{
	if (!name.isEmpty()) {
		if (_actions.size() > 0) {
			action act = _actions.last();
			act.add(name);
		} else {
			a(name);
		}
	}
	return *this;
}

/**
 * @fn router & router::syn (const string & action)
 * @brief Synonym for router::synonym(const string &).
 * @see router::synonym(const string &)
 */

/**
 * @brief Adds synonym for action.
 *
 * @param action
 * 		Synonym for action. Action must be Latin1 encoded
 * @return Reference to router itself.
 *
 * @see router::synonym(const string &)
 */
router & router::synonym (const char * action)
{
	return synonym(string::fromLatin1(action));
}

/**
 * @fn router & router::syn (const char * action)
 * @brief Synonym for router::synonym(const char *).
 * @see router::synonym(const char *)
 */


template <typename T>
router & router::o (const string & optname)
{
	option opt = make_option<T>(optname);
	_options.insert(optname, opt);
	return *this;
}

template <typename T>
router & router::o (const string & optname, const T & defaultValue)
{
	option opt = make_option<T>(optname);
	opt.setDefault(defaultValue);
	_options.insert(optname, opt);
	return *this;
}

//template <typename T>
//router & router::o (const string & optname, const string & description)
//{
//	option opt = make_option<T>(optname);
//	opt.setDescription(description);
//	_options.insert(optname, opt);
//	return *this;
//}

router & router::b (const string & optname)
{
	return o<bool>(optname);
}

router & router::i (const string & optname)
{
	return o<integral_t>(optname);
}

router & router::n (const string & optname)
{
	return o<real_t>(optname);
}

router & router::s (const string & optname)
{
	return o<string>(optname);
}

router & router::b (const string & optname, bool defaultValue)
{
	return o<bool>(optname, defaultValue);
}

router & router::i (const string & optname, integral_t defaultValue)
{
	return o<integral_t>(optname, defaultValue);
}

router & router::n (const string & optname, real_t defaultValue)
{
	return o<real_t>(optname, defaultValue);
}

router & router::s (const string & optname, const string & defaultValue)
{
	return o<string>(optname, defaultValue);
}

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


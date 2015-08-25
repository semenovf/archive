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
 * @param action Action.
 * @return Router.
 */
router & router::a (const string & action)
{
	if (action.isEmpty())
		return a(action::any());

	stringlist actions;
	actions.append(action);
	return a(actions);
}

router & router::a (const char * action)
{
	if (!action)
		return a(action::any());
	return a(string::fromLatin1(action));
}

/**
 * @brief Adds synonym for action.
 *
 * @param action Synonym for action.
 * @return Reference to router itself.
 *
 * @see router::synonym(const char *)
 */
router & router::synonym (const string & action)
{
	if (!action.isEmpty()) {
		if (_actions.size() > 0) {
			stringlist a = _actions.last();
			a.append(action);
		} else {
			a(action);
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

#ifdef __COMMENT__
router & router::b (const string & optname)
{
	_activeOptdata(optdata()); // reset
	_options.insert(optname, _activeOptdata);
	return *this;
}

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


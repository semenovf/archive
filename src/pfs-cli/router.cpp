/*
 * router.cpp
 *
 *  Created on: Aug 20, 2015
 *      Author: wladt
 */

#include "pfs/cli.hpp"

namespace pfs { namespace cli {

const vector<string> router::AnyAction = vector<string>();

/**
 * @brief Appends action (first element of vector) and it's alternatives.
 *
 * @param alts Contains action and it's alternatives.
 *        @c alts can be null value means any action (@see AnyAction)
 * @return Reference to router itself.
 */
router & router::a (const vector<string> & alts)
{
	_actions.append(alts);
	return *this;
}

/**
 * @brief Appends action.
 *
 * @param action Action.
 * @return Router.
 */
router & router::a (const string & action)
{
	if (action.isEmpty())
		return a(AnyAction);

	vector<string> actions;
	actions.append(action);
	return a(actions);
}

router & router::a (const char * action)
{
	if (!action)
		return a(AnyAction);
	return a(string::fromLatin1(action));
}

/**
 * @brief Adds synonym for action.
 *
 * @param action Synonym for action.
 * @return Reference to router itself.
 *
 * @see router::synonym(const char * action)
 */
router & router::synonym (const string & action)
{
	if (!action.isEmpty()) {
		if (_actions.size() > 0) {
			vector<string> a = _actions.last();
			a.append(action);
		} else {
			a(action);
		}
	}
	return *this;
}

/**
 * @brief Adds synonym for action.
 *
 * @param action
 * 		Synonym for action.
 * @return Reference to router itself.
 *
 * @see router::synonym(const string & action)
 */
router & router::synonym (const char * action)
{
	return synonym(string::fromLatin1(action));
}

}} // pfs::cli


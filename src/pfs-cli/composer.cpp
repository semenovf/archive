/*
 * composer.cpp
 *
 *  Created on: Aug 27, 2015
 *      Author: wladt
 */

#include "pfs/cli/composer.hpp"

namespace pfs { namespace cli {

/**
 * @brief Appends action.
 *
 * @param name
 * 		Action name.
 * @return Reference to router itself.
 */
composer & composer::a (const string & name)
{
	_prouter->appendAction(
			name.isEmpty()
			? action::any()
			: action(name));
	_state = StateAction;
	_action = &
	return *this;
}

composer & composer::a (const char * name)
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
composer & composer::synonym (const string & name)
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
router & composer::composer (const char * action)
{
	return synonym(string::fromLatin1(action));
}

/**
 * @fn router & router::syn (const char * action)
 * @brief Synonym for router::synonym(const char *).
 * @see router::synonym(const char *)
 */

}} // pfs::cli


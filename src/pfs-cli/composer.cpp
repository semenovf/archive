/*
 * composer.cpp
 *
 *  Created on: Aug 27, 2015
 *      Author: wladt
 */

#include "pfs/cli/action.hpp"
#include "pfs/cli/router.hpp"
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
	_state  = StateAction;
	_paction = _prouter->lastActionPtr();
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
	PFS_ASSERT_NULLPTR(_paction);
	PFS_ASSERT(_state == StateAction);
	_paction->add(name);
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
composer & composer::synonym (const char * action)
{
	return synonym(string::fromLatin1(action));
}

/**
 * @fn router & router::syn (const char * action)
 * @brief Synonym for router::synonym(const char *).
 * @see router::synonym(const char *)
 */


template <typename T>
composer & composer::o (const string & optname)
{
	option opt = make_option<T>(optname);
	_prouter->insertOption(optname, opt);
	_poption = _prouter->optionPtr(optname);
	_state  = StateOption;
	return *this;
}


composer & composer::b (const string & optname)
{
	return o<bool>(optname);
}

}} // pfs::cli


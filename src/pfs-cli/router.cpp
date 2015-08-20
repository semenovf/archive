/*
 * router.cpp
 *
 *  Created on: Aug 20, 2015
 *      Author: wladt
 */

#include "pfs/cli.hpp"

namespace pfs { namespace cli {

router & router::a (const vector<string> & actions)
{
	_actions.append(actions);
	return *this;
}

router & router::a (const string & action)
{
	vector<string> actions;
	actions.append(action);
	return a(actions);
}

router & router::a (const char * action)
{
	return a(string::fromLatin1(action));
}

router & router::alt (const string & action)
{
	if (_actions.size() > 0) {
		vector<string> a = _actions.last();
		a.append(action);
	} else {
		a(action);
	}
	return *this;
}

router & router::alt (const char * action)
{
	return alt(string::fromLatin1(action));
}


}} // pfs::cli


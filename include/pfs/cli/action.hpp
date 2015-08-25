/*
 * action.hpp
 *
 *  Created on: Aug 25, 2015
 *      Author: wladt
 */

#ifndef __PFS_CLI_ACTION_HPP__
#define __PFS_CLI_ACTION_HPP__

#include <pfs/stringlist.hpp>

namespace pfs { namespace cli {

class DLL_API action
{
	stringlist _synonyms;

public:
	action () {}

	bool isAny () const { return _synonyms.isEmpty(); }

	action (const string & name)
	{
		add(name);
	}

	action & add (const string & name)
	{
		_synonyms.append(name);
		return *this;
	}

	void setName (const string & name)
	{
		if (_synonyms.size() > 0)
			_synonyms[0] = name;
		else
			_synonyms.append(name);
	}

	void clear ()
	{
		_synonyms.clear();
	}

	static action any ()
	{
		return action();
	}
};

}} // pfs::cli


#endif /* __PFS_CLI_ACTION_HPP__ */

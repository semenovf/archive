/*
 * path.cpp
 *
 *  Created on: Nov 7, 2015
 *      Author: wladt
 */

#include <pfs/algo/string/split.hpp>
#include "pfs/fs/path.hpp"

namespace pfs { namespace fs {

path::path (const string & s)
{
	split(s.begin(), s.end()
		, _separator.begin(), _separator.end()
		, DontKeepEmpty, _components);
}

}} // pfs::fs



/**
 * @file stringlist.hpp
 * @author wladt
 * @date Apr 3, 2014
 */

#ifndef __PFS_STRINGLIST_HPP__
#define __PFS_STRINGLIST_HPP__

#include <pfs/string.hpp>
#include <pfs/vector.hpp>

namespace pfs {

typedef pfs::vector<string> stringlist;

inline stringlist & operator << (stringlist & sl, const string & s)
{
	sl.push_back(s);
	return sl;
}

inline stringlist & operator << (stringlist & sl, const char * latin1)
{
	sl.push_back(string(latin1));
	return sl;
}

inline stringlist & operator << (stringlist & sl, const stringlist & other)
{
	stringlist::const_iterator it = other.begin();
	stringlist::const_iterator it_end = other.end();

	for (; it != it_end; ++it)
		sl.push_back(*it);
	return sl;
}


} // pfs

#endif // __PFS_STRINGLIST_HPP__

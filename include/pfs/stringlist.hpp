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

typedef string::stringlist stringlist;

inline stringlist & operator << (stringlist & sl, const string & s)
{
	sl.append(s);
	return sl;
}

inline stringlist & operator << (stringlist & sl, const char * latin1)
{
	sl.append(string::fromLatin1(latin1));
	return sl;
}

inline stringlist & operator << (stringlist & sl, const stringlist & other)
{
	sl.append(other);
	return sl;
}


} // pfs

#endif // __PFS_STRINGLIST_HPP__

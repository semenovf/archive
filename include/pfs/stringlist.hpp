/**
 * @file stringlist.hpp
 * @author wladt
 * @date Mar 19, 2014
 */

#ifndef __PFS_STRINGLIST_HPP__
#define __PFS_STRINGLIST_HPP__

#include <pfs/string.hpp>
#include <pfs/vector.hpp>

namespace pfs {

// TODO be need to derive from pfs::list (analog for Qt::QList)
// TODO pfs::list is not implemented yet
class stringlist : public pfs::vector<pfs::string>
{
	typedef pfs::vector<pfs::string> base_class;
public:
	stringlist () : base_class() {}
	stringlist (const pfs::string & str) : base_class() { append(str); }
	stringlist (const stringlist & other) : base_class(other) {}

	stringlist	 operator  + ( const stringlist & other ) const
	{
		stringlist r(*this);
		r.append(other);
		return r;
	}

	stringlist & operator << ( const pfs::string & str ) { append(str); return *this; }
	stringlist & operator << ( const char * latin1 )     { append(_l1(latin1)); return *this; }
	stringlist & operator << (const stringlist & other)  { append(other); return *this; }
};

} // pfs

#endif // __PFS_STRINGLIST_HPP__

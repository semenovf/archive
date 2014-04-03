/**
 * @file stringlist_basic.hpp
 * @author wladt
 * @date Mar 19, 2014
 */

#ifndef __PFS_BITS_STRINGLIST_BASIC_HPP__
#define __PFS_BITS_STRINGLIST_BASIC_HPP__

#include <pfs/vector.hpp>

namespace pfs {

// TODO be need to derive from pfs::list (analog for Qt::QList)
// TODO pfs::list is not implemented yet
template <typename string_type>
class stringlist_basic : public pfs::vector<string_type>
{
	typedef pfs::vector<string_type> base_class;
public:
	stringlist_basic () : base_class() {}
	stringlist_basic (const string_type & str) : base_class() { append(str); }
	stringlist_basic (const stringlist_basic & other) : base_class(other) {}

	stringlist_basic operator  + (const stringlist_basic & other) const
	{
		stringlist_basic r(*this);
		r.append(other);
		return r;
	}

	stringlist_basic & operator << ( const string_type & str )      { append(str); return *this; }
	stringlist_basic & operator << ( const char * latin1 )          { append(string_type::fromLatin1(latin1)); return *this; }
	stringlist_basic & operator << (const stringlist_basic & other) { append(other); return *this; }
};

} // pfs

#endif // __PFS_BITS_STRINGLIST_BASIC_HPP__

/**
 * @file stringlist.hpp
 * @author wladt
 * @date Apr 3, 2014
 */

#ifndef __PFS_BITS_STRINGLIST_HPP__
#define __PFS_BITS_STRINGLIST_HPP__

#include <pfs/vector.hpp>

namespace pfs {

template <typename StringT>
class stringlist_basic : public pfs::vector<StringT>
{
	typedef pfs::vector<StringT> base_class;
public:
	stringlist_basic () : base_class() {}
//	stringlist_basic (const string_type & str) : base_class() { append(str); }
	stringlist_basic (const stringlist_basic & other) : base_class(other) {}

//	stringlist_basic operator + (const stringlist_basic & other) const
//	{
//		stringlist_basic r(*this);
//		r.append(other);
//		return r;
//	}

	stringlist_basic & operator << (const StringT & s)              { this->append(s); return *this; }
	stringlist_basic & operator << (const char * latin1)            { this->append(StringT::fromLatin1(latin1)); return *this; }
	stringlist_basic & operator << (const stringlist_basic & other) { this->append(other); return *this; }
};

} // pfs

#endif // __PFS_BITS_STRINGLIST_HPP__

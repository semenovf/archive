/*
 * list.hpp
 *
 *  Created on: Nov 21, 2013
 *      Author: wladt
 */

#ifndef __PFS_LIST_HPP__
#define __PFS_LIST_HPP__

#include <list>

namespace pfs {

template <typename T>
class list : protected std::list<T>
{
	typedef std::list<T> base_class;

public:
	typedef typename base_class::iterator iterator;
	typedef typename base_class::const_iterator const_iterator;

public:
	list () : base_class() {}
};

} // pfs

#endif /* __PFS_LIST_HPP__ */

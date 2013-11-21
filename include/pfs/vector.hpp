/*
 * vector.hpp
 *
 *  Created on: Nov 21, 2013
 *      Author: wladt
 */

#ifndef __PFS_VECTOR_HPP__
#define __PFS_VECTOR_HPP__

#include <vector>

namespace cwt {

template <typename T>
class vector : protected std::vector<T>
{
	typedef std::vector<T> base_class;

public:
	typedef typename base_class::iterator iterator;
	typedef typename base_class::const_iterator const_iterator;

public:
	vector () : base_class() {}
};


} // cwt

#endif /* __PFS_VECTOR_HPP__ */

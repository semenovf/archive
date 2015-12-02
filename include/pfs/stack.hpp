/*
 * stack.hpp
 *
 *      Author: wladt
 */

#ifndef __PFS_STACK_HPP__
#define __PFS_STACK_HPP__

#if __cplusplus >= 201103L

#include <stack>

namespace pfs {
typename _Tp,
template <typename T, typename Sequence = deque<T> >
using stack = std::stack<T, Sequence>;

} // pfs

#else // !C++11

namespace pfs {

#include <stack>

using std;

} // pfs

#endif /* __PFS_STACK_HPP__ */

/*
 * stack.hpp
 *
 *      Author: wladt
 */

#ifndef __PFS_STACK_HPP__
#define __PFS_STACK_HPP__

#include <stack>

namespace pfs {

template<typename T, typename Sequence = std::deque<T> >
struct stack
{
	typedef ::std::stack<T, Sequence> type;
};

} // pfs

#endif /* __PFS_STACK_HPP__ */

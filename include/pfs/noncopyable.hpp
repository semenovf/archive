/**
 * @file noncopyable.hpp
 * @author wladt
 * @date Mar 17, 2014
 */

#ifndef __PFS_NONCOPYABLE_HPP__
#define __PFS_NONCOPYABLE_HPP__

namespace pfs {

class noncopyable
{
protected:
	noncopyable() {}
	~noncopyable() {}

private:
	noncopyable (const noncopyable &);
	noncopyable & operator = (const noncopyable &);
};

typedef noncopyable deny_copy;

} // pfs

#endif /* __PFS_NONCOPYABLE_HPP__ */

/**
 * @file   atomic.hpp
 * @author wladt
 * @date   Mar 26, 2013 12:24:30 PM
 *
 * @brief
 */

#ifndef __CWT_ATOMIC_HPP__
#define __CWT_ATOMIC_HPP__

#include <cwt/cwt.h>

CWT_NS_BEGIN

class atomic_int
{
public:
	void store(int v);
	int  load();
	void ref();
	int  deref();
private:
	int m_value;
};

CWT_NS_END

#endif /* __CWT_ATOMIC_HPP__ */

/**
 * @file   writer.hpp
 * @author wladt
 * @date   Mar 7, 2013 8:56:07 AM
 *
 * @brief
 */

#ifndef __CWT_WRITER_HPP__
#define __CWT_WRITER_HPP__

#include <cwt/cwt.h>

CWT_NS_BEGIN

class Writer {
protected:
	Writer() {}
public:
	virtual ~Writer() {}
	virtual bool    isNull() const = 0;
	virtual void 	close() = 0;
	virtual void    flush() = 0;
	virtual ssize_t write(const Char chars[], size_t sz) = 0;
};

CWT_NS_END

#endif /* __CWT_WRITER_HPP__ */

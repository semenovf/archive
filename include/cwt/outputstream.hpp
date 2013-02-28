/**
 * @file   outputsream.hpp
 * @author wladt
 * @date   Feb 28, 2013 12:21:01 PM
 *
 * @brief
 */

#ifndef __CWT_OUTPUTSTREAM_HPP__
#define __CWT_OUTPUTSTREAM_HPP__

#include <cwt/cwt.h>

CWT_NS_BEGIN

class OutputStream {
protected:
	OutputStream() {}
public:
	virtual ~OutputStream() {}

	virtual bool isNull() const = 0;
	virtual void close()  = 0;
	virtual void flush()  = 0;
	virtual ssize_t write(const char bytes[], size_t sz) = 0;
};

CWT_NS_END

#endif /* __CWT_OUTPUTSTREAM_HPP__ */

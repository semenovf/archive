/**
 * @file   inputstream.hpp
 * @author wladt
 * @date   Feb 28, 2013 11:48:00 AM
 *
 * @brief
 */

#ifndef __CWT_INPUTSTREAM_HPP__
#define __CWT_INPUTSTREAM_HPP__

#include <cwt/cwt.h>

CWT_NS_BEGIN

class InputStream {
protected:
	InputStream() {}
public:
	virtual ~InputStream() {}

	virtual bool isNull() const = 0;
	virtual ssize_t available() = 0;
	virtual void close() = 0;
	virtual ssize_t read(byte_t bytes[], size_t szMax) = 0;
};

CWT_NS_END

#endif /* __CWT_INPUTSTREAM_HPP__ */

/**
 * @file   reader.hpp
 * @author wladt
 * @date   Feb 27, 2013 3:01:42 PM
 *
 * @brief
 */

#ifndef __CWT_READER_HPP__
#define __CWT_READER_HPP__

#include <cwt/cwt.h>
#include <cwt/char.hpp>

CWT_NS_BEGIN

class Reader {
protected:
	Reader() {}
public:
	virtual ~Reader() {}
	virtual bool    isNull() const = 0;
	virtual void 	close() = 0;
	virtual ssize_t read(Char chars[], size_t sz)  = 0;
};

CWT_NS_END

#endif /* __CWT_READER_HPP__ */

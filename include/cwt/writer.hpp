/*
 * writer.hpp
 *
 *  Created on: Jul 11, 2013
 *      Author: wladt
 */

#ifndef __CWT_IO_WRITER_HPP__
#define __CWT_IO_WRITER_HPP__

#include <cwt/io.hpp>
#include <cwt/shared_ptr.hpp>

CWT_NS_BEGIN

namespace io {

template <typename T>
struct Consumer
{
	bool put(T & ch);
	ssize_t write(T * chars, size_t size);
};


template <typename T>
struct Writer
{
	bool put(T & ch);
	ssize_t write(T * chars, size_t size);
};

} // namespace io

CWT_NS_END

#endif /* __CWT_IO_WRITER_HPP__ */

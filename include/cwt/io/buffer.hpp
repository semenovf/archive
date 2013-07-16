/*
 * buffer.hpp
 *
 *  Created on: Jul 16, 2013
 *      Author: wladt
 */

#ifndef __CWT_IO_BUFFER_HPP__
#define __CWT_IO_BUFFER_HPP__

#include <cwt/io/device.hpp>

CWT_NS_BEGIN

namespace io {

class DLL_API Buffer : public Device
{
protected:
	virtual ssize_t readBytes      (char bytes[], size_t n);
	virtual ssize_t writeBytes     (const char bytes[], size_t n) { CWT_ASSERT(n <= CWT_SSIZE_MAX); m_buffer.append(bytes, n); return ssize_t(n); }
	virtual size_t  bytesAvailable () const { return m_buffer.size(); }

public:
	Buffer() : m_buffer() {}
	virtual int  close  () { return 1; }
	virtual bool opened () const { return true; }
	virtual void flush  () {}

private:
	Vector<char> m_buffer;
};


ssize_t Buffer::readBytes (char bytes[], size_t n)
{
	size_t nbytes = 0;
	if (n > 0 && m_buffer.size() > 0) {
		nbytes = CWT_MIN(n, m_buffer.size());
		CWT_ASSERT(nbytes <= CWT_SSIZE_MAX);
		memcpy(bytes, m_buffer.constData(), nbytes);
		m_buffer.remove(0, nbytes);
	}
	return ssize_t(nbytes);
}

} // namespace io

CWT_NS_END

#endif /* __CWT_IO_BUFFER_HPP__ */

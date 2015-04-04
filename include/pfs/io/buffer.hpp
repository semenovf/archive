/*
 * buffer.hpp
 *
 *  Created on: Jul 16, 2013
 *      Author: wladt
 */

#ifndef __CWT_IO_BUFFER_HPP__
#define __CWT_IO_BUFFER_HPP__

#include <cwt/io/device.hpp>

namespace cwt { namespace io {

class DLL_API buffer : public device
{
	pfs::vector<char> m_buffer;

protected:
	virtual ssize_t readBytes      (char bytes[], size_t n);
	virtual ssize_t writeBytes     (const char bytes[], size_t n);
	virtual size_t  bytesAvailable () const { return m_buffer.size(); }
	virtual bool    closeDevice    () { return true; }
	virtual bool    deviceIsOpened () const { return true; }
	virtual void    flushDevice    () {}

public:
	buffer() : m_buffer() {}
	virtual ~buffer() {}

	const pfs::vector<char> & data () const { return m_buffer; }
};


inline ssize_t buffer::readBytes (char bytes[], size_t n)
{
	size_t nbytes = 0;
	if (n > 0 && m_buffer.size() > 0) {
		nbytes = pfs::min(n, m_buffer.size());
		PFS_ASSERT(nbytes <= PFS_SSIZE_MAX);
		memcpy(bytes, m_buffer.constData(), nbytes);
		m_buffer.remove(0, nbytes);
	}
	return ssize_t(nbytes);
}

inline ssize_t buffer::writeBytes (const char bytes[], size_t n)
{
	PFS_ASSERT(n <= PFS_SSIZE_MAX);
	m_buffer.append(bytes, n);
	return ssize_t(n);
}

}} // cwt::io

#endif /* __CWT_IO_BUFFER_HPP__ */

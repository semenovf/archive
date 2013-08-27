/*
 * device.cpp
 *
 *  Created on: Aug 27, 2013
 *      Author: wladt
 */

#include "../include/cwt/io/device.hpp"

CWT_NS_BEGIN

namespace io {

ssize_t Device::read (ByteArray & ba, size_t n)
{
	CWT_ASSERT(n <= CWT_SSIZE_MAX);
	if (!m_inputBuffer) {
		m_inputBuffer = new char[n];
		m_inputBufferSize = n;
	}

	if (m_inputBufferSize < n) {
		delete[] m_inputBuffer;
		m_inputBuffer = new char[n];
		m_inputBufferSize = n;
	}

	ssize_t rb = readBytes(m_inputBuffer, n);
	if (rb > 0)
		ba.append(m_inputBuffer, size_t(rb));

	return rb;
}

} // namespace io

CWT_NS_END




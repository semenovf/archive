/*
 * device.cpp
 *
 *  Created on: Aug 27, 2013
 *      Author: wladt
 */

#include "../include/cwt/io/device.hpp"

namespace cwt { namespace io {

ssize_t device::read (pfs::bytearray & ba, size_t n)
{
	PFS_ASSERT(n <= PFS_SSIZE_MAX);
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

}} // cwt::io




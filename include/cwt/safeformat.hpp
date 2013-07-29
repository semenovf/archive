/*
 * safeformat.hpp
 *
 *  Created on: Jul 29, 2013
 *      Author: wladt
 */

#ifndef __CWT_SAFEFORMAT_HPP__
#define __CWT_SAFEFORMAT_HPP__

#include <cwt/cwt.hpp>

CWT_NS_BEGIN

template <typename _P>
class SafeFormat
{
	typedef typename _P::const_iterator const_iterator;

public:
	SafeFormat(const _P & format)
		: m_format(format)
		, m_result()
		, m_cursor(format.begin())
		, m_end(format.end())
	{}

/*
	_P & operator () () { return m_result; }
	const _P & operator () () const { return m_result; }
*/

	inline operator String & ()
	{
		if (m_cursor < m_end)
			m_result.append(_P(m_cursor, m_end));
		return m_result;
	}


	SafeFormat & operator () (char c) { m_result.append(_P(1, c)); return *this; }
	SafeFormat & operator () (const char * s) { m_result.append(_P(s)); return *this; }

private:
	_P m_format;
	_P m_result;
	const_iterator m_cursor;
	const_iterator m_end;
};

CWT_NS_END

#endif /* __CWT_SAFEFORMAT_HPP__ */

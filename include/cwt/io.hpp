/*
 * producer.hpp
 *
 *  Created on: Jul 11, 2013
 *      Author: wladt
 */

#ifndef __CWT_IO_HPP__
#define __CWT_IO_HPP__

#include <cwt/cwt.hpp>
#include <cwt/vector.hpp>

CWT_NS_BEGIN

namespace io {

template <typename T1, typename T2>
struct Codec
{
	Vector<T2> convert(const Vector<T1> & inBuffer, ssize_t * nonconverted);
};

template <typename T>
struct Filter
{
	Vector<T> process(const Vector<T> & input, bool * ok);
};

template <typename T>
class FilterChain
{
	typedef Filter<T> filter_type;

public:
	FilterChain() {}
	void      append(shared_ptr<filter_type> filter) { m_chain.append(filter); }
	void      clear() { m_chain.clear; }
	Vector<T> process(const Vector<T> & input, bool * ok);

private:
	Vector<filter_type> m_chain;
};

template <typename T>
Vector<T> FilterChain<T>::process(const Vector<T> & input, bool * ok)
{
	Vector<filter_type>::iterator it = m_chain.begin();
	Vector<filter_type>::iterator itEnd = m_chain.end();
	Vector<T> r(input);

	bool okk = true;

	while (okk && it != itEnd) {
		r = it->process(r, &okk);
		++it;
	}

	if (ok)
		*ok = okk;

	return r;
}


} // namespace io

CWT_NS_END


#endif /* __CWT_IO_HPP__ */

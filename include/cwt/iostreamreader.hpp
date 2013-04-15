/**
 * @file   iostreamreader.hpp
 * @author wladt
 * @date   Mar 4, 2013 12:16:56 PM
 *
 * @brief
 */

#ifndef __CWT_IOSTREAMREADER_HPP__
#define __CWT_IOSTREAMREADER_HPP__

#include <cwt/cwt.h>
#include <cwt/iostream.hpp>
#include <cwt/textcodec.hpp>

CWT_NS_BEGIN

class InputStreamReader {
protected:
	InputStreamReader() : m_is(NULL), m_decoder(NULL) {}
public:
	InputStreamReader(InputStream &in, const char *charset = NULL, Char replacement = Char::ReplacementChar);
	virtual ~InputStreamReader() { close(); }
	virtual bool    isNull() const { return !m_is ? true : m_is->isNull() ? true : false; }
	virtual void 	close();
	virtual ssize_t read(Char chars[], size_t len);

	String          readAll();
private:
	InputStream   *m_is;
	StreamDecoder *m_decoder;
};


CWT_NS_END

#endif /* __CWT_IOSTREAMREADER_HPP__ */

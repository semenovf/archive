/**
 * @file   Char.hpp
 * @author wladt
 * @date   Jan 24, 2013 3:51:54 PM
 *
 * @brief
 */

#ifndef __CWT_CHAR_HPP__
#define __CWT_CHAR_HPP__

#include <cwt/cwt.h>

CWT_NS_BEGIN

// Qt   QString
// C#
// Java

class DLL_API Char {
public:
	Char() : ch(0x0) {}
	Char(uc_char16_t code)  : ch((uc_uchar_t)code) {}
	Char(uc_uchar16_t code) : ch((uc_uchar_t)code) {}
	Char(uc_char32_t code)  : ch((uc_uchar_t)code) {}
	Char(uc_uchar32_t code) : ch((uc_uchar_t)code) {}
	~Char() {}

	operator uc_uchar16_t() { return (uc_uchar16_t)ch; }
	operator uc_uchar32_t() { return (uc_uchar32_t)ch; }

	uc_uchar_t unicode() const { return ch; }

	friend bool	operator != (Char ch1, Char ch2) { return ch1.ch != ch2.ch; }
	friend bool	operator  < (Char ch1, Char ch2) { return ch1.ch  < ch2.ch; }
	friend bool	operator <= (Char ch1, Char ch2) { return ch1.ch <= ch2.ch; }
	friend bool	operator == (Char ch1, Char ch2) { return ch1.ch == ch2.ch; }
	friend bool	operator >  (Char ch1, Char ch2) { return ch1.ch  > ch2.ch; }
	friend bool	operator >= (Char ch1, Char ch2) { return ch1.ch >= ch2.ch; }

private:
	uc_uchar_t ch;
};

CWT_NS_END

#endif /* __CWT_CHAR_HPP__ */

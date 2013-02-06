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
#include <QtCore/QChar>

CWT_NS_BEGIN

// Qt   QString
// C#
// Java

class DLL_API Char {
public:
	Char();
	Char(uc_char16_t code);
	Char(uc_uchar16_t code);
	Char(uc_char32_t code);
	Char(uc_uchar32_t code);
	Char(char ch);
	Char(uchar_t ch);

	operator uc_uchar16_t() { return (uc_uchar16_t)__impl.unicode(); }
	operator uc_uchar32_t() { return (uc_uchar32_t)__impl.unicode(); }

	friend bool	operator!=(Char ch1, Char ch2);
	friend bool	operator<(Char ch1, Char ch2);
	friend bool	operator<=(Char ch1, Char ch2);
	friend bool	operator==(Char ch1, Char ch2);
	friend bool	operator>(Char ch1, Char ch2);
	friend bool	operator>=(Char ch1, Char ch2);
public:
	QChar __impl;
};

CWT_NS_END

#endif /* __CWT_CHAR_HPP__ */

/**
 * @file   char.hpp
 * @author wladt
 * @date   Jan 24, 2013 3:56:14 PM
 *
 * @brief
 */

#include <cwt/char.hpp>

CWT_NS_BEGIN

Char::Char() : __impl() {}
Char::Char(uc_char16_t code) : __impl(code) {}
Char::Char(uc_uchar16_t code) : __impl(code) {}
Char::Char(uc_char32_t code) : __impl(code) {}
Char::Char(uc_uchar32_t code) : __impl(code) {}
Char::Char(char ch)  : __impl(ch) {}
Char::Char(uchar_t ch) : __impl(ch) {}

bool operator!=(Char ch1, Char ch2) { return ch1.__impl != ch2.__impl ? true : false; }
bool operator<(Char ch1, Char ch2)  { return ch1.__impl < ch2.__impl ? true : false; }
bool operator<=(Char ch1, Char ch2) { return ch1.__impl <= ch2.__impl ? true : false; }
bool operator==(Char ch1, Char ch2) { return ch1.__impl == ch2.__impl ? true : false; }
bool operator>(Char ch1, Char ch2)  { return ch1.__impl > ch2.__impl ? true : false; }
bool operator>=(Char ch1, Char ch2) { return ch1.__impl >= ch2.__impl ? true : false; }


CWT_NS_END



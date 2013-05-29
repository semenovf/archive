/**
 * @file   string_impl.hpp
 * @author wladt
 * @date   Feb 27, 2013 11:16:35 AM
 *
 * @brief
 */

#ifndef __CWT_STRING_QT_HPP__
#define __CWT_STRING_QT_HPP__

#include "../../include/cwt/string.hpp"
#define QT_NO_CAST_FROM_ASCII
#include <QtCore/QString>

CWT_NS_BEGIN

class String::Impl : public QString {
public:
	Impl() : QString() {}
	Impl(const Char *unicode, int size);
	Impl(Char ch)                 : QString(QChar(ch.unicode())) {}
	Impl(int size, Char ch)       : QString(size, QChar(ch.unicode())) {}
	Impl(const Impl &other)       : QString(other) {}
	Impl(const QString &other)    : QString(other) {}
	Impl& operator=(Char ch)      { QString::operator  = (QChar(ch.unicode())); return *this; }
	~Impl() { QChar *d = data(); memset(d, 0, sizeof(QChar) * length()); }
};

inline String::Impl::Impl(const Char *unicode, int size) : QString()
{
	resize(size);
	QChar *chars = data();

	for(int i = 0; i < size; i++)
		chars[i] = QChar(unicode[i].unicode());
}
CWT_NS_END

#endif /* __CWT_STRING_QT_HPP__ */

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
#include <QString>

CWT_NS_BEGIN

class String::Impl : public QString {
public:
	Impl() : QString() {}
	Impl(const Char *unicode);
	Impl(const Char *unicode, size_t size);
	Impl(Char ch)                 : QString(QChar(ch.unicode())) {}
	Impl(int size, Char ch)       : QString(size, QChar(ch.unicode())) {}
	Impl(const Impl &other)       : QString(other) {}
	Impl(const QString &other)    : QString(other) {}
	Impl& operator=(Char ch)      { QString::operator  = (QChar(ch.unicode())); return *this; }
	~Impl() { QChar *d = data(); memset(d, 0, sizeof(QChar) * length()); }
};

inline String::Impl::Impl(const Char *unicode) : QString()
{
	while(*unicode != Char(0)) {
		this->append(QChar(unicode->unicode()));
		++unicode;
	}
}

inline String::Impl::Impl(const Char *unicode, size_t size) : QString()
{
	CWT_ASSERT(size <= CWT_INT_MAX);
	resize(int(size));
	QChar *chars = data();

	for(size_t i = 0; i < size; i++)
		chars[i] = QChar(unicode[i].unicode());
}

CWT_NS_END

#endif /* __CWT_STRING_QT_HPP__ */

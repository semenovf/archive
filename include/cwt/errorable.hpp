/**
 * @file   errorable.hpp
 * @author wladt
 * @date   Jan 31, 2013 2:34:11 PM
 *
 * @brief
 */

#ifndef __CWT_ERRORABLE_HPP__
#define __CWT_ERRORABLE_HPP__

#include <cwt/cwt.h>
#include <cwt/string.hpp>
#include <cwt/vector.hpp>
#include <cwt/logger.hpp>
#include <cwt/system.h>

CWT_NS_BEGIN

struct ErrorData {
	ErrorData() : ntimes(0), errstr() {}
	ErrorData(const String& s) : ntimes(0), errstr(s) {}
	int    ntimes;
	String errstr;
};

class DLL_API Errorable
{
	static const int SystemErrorBufLen = 128;
protected:
	Errorable() {}

public:
	virtual ~Errorable() { clearErrors(); }
	/*virtual ~Errorable() {}*/
	void addSystemError(int errn, const char *prefix = 0);
	void addSystemError(int errn, const String &prefix);
	void addError(const char *text);
	void addError(const String& text);
	void clearErrors() { m_errors.clear(); }
	size_t errorCount() const;
	const String& errorTextAt(size_t index) const;
	bool isError() const { return m_errors.size() != 0; }
	bool isGood() const { return m_errors.size() == 0; }
	const String& lastErrorText() const;
	void logErrors() const;

private:
	Vector<ErrorData> m_errors;
};

inline size_t Errorable::errorCount() const
{
	return m_errors.size();
}

inline const String& Errorable::errorTextAt(size_t index) const
{
	return index < m_errors.size()
			? m_errors[index].errstr
			: String::null();
}

inline const String& Errorable::lastErrorText() const
{
	return m_errors.size() > 0
			? m_errors.last().errstr
			: String::null();
}

inline void Errorable::addError(const String& text)
{
	if (m_errors.size() > 0 && m_errors.last().errstr == text)
		m_errors.last().ntimes++;
	else
		m_errors.append(ErrorData(text));
}

inline void Errorable::addError(const char *text)
{
	addError(String::fromUtf8(text));
}

inline void Errorable::addSystemError(int errn, const String &prefix)
{
	char errstr[SystemErrorBufLen];
	cwt_strerror(errn, errstr, SystemErrorBufLen);
	if (prefix.isEmpty()) {
		addError(String::fromUtf8(errstr));
	} else {
		addError(String().sprintf("%ls: %s", prefix.unicode(), errstr));
	}
}

inline void Errorable::addSystemError(int errn, const char *prefix)
{
	if (prefix)
		addSystemError(errn, String::null());
	else
		addSystemError(errn, String::fromUtf8(prefix));
}

inline void Errorable::logErrors() const
{
	Vector<ErrorData>::const_iterator itEnd = m_errors.end();
	for (Vector<ErrorData>::const_iterator it = m_errors.begin()
			; it != itEnd; ++it) {
		if (it->ntimes > 1) {
			Logger::error("%ls", it->errstr.unicode());
		} else {
			Logger::error(_Tr("%ls <occured %d times>"), it->errstr.unicode(), it->ntimes);
		}
	}
}

CWT_NS_END

#endif /* __CWT_ERRORABLE_HPP__ */

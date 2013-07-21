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
#include <cwt/string.h>

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
	void addSystemError(int errn, const char *prefix, ...);
	void addSystemError(int errn, const String &prefix);
	void addError(const char * cformat, ...);
	void addError(const String& text);
	void clearErrors() { m_errors.clear(); }
	size_t errorCount() const;
	const String& errorTextAt(size_t index) const;
	bool isError() const { return m_errors.size() != 0; }
	bool isGood() const { return m_errors.size() == 0; }
	const String& lastErrorText() const;
	void logErrors(bool clear = true);

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
			: String::constNull();
}

inline const String& Errorable::lastErrorText() const
{
	return m_errors.size() > 0
			? m_errors.last().errstr
			: String::constNull();
}

inline void Errorable::addError(const String& text)
{
	if (m_errors.size() > 0 && m_errors.last().errstr == text) {
		;
	} else {
		m_errors.append(ErrorData(text));
	}
	m_errors.last().ntimes++;
}

CWT_NS_END

#endif /* __CWT_ERRORABLE_HPP__ */

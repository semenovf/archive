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

CWT_NS_BEGIN

struct ErrorData {
	ErrorData() : ntimes(0), errstr() {}
	ErrorData(const String& s) : ntimes(0), errstr(s) {}
	int    ntimes;
	String errstr;
};

class DLL_API Errorable
{
protected:
	Errorable() {}

public:
	/*virtual ~Errorable() {}*/

	void addError(const String& text);
	void clearErrors() { m_errors.clear(); }
	size_t errorCount() const;
	const String& errorTextAt(int index) const;
	bool isError() const { return m_errors.size() != 0; }
	bool isGood() const { return m_errors.size() == 0; }
	const String& lastErrorText() const;

private:
	Vector<ErrorData> m_errors;
};

inline size_t Errorable::errorCount() const
{
	return m_errors.size();
}

inline const String& Errorable::errorTextAt(int index) const
{
	return index >= 0 && index < m_errors.size()
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

CWT_NS_END

#endif /* __CWT_ERRORABLE_HPP__ */

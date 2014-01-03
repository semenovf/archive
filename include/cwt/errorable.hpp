/**
 * @file   errorable.hpp
 * @author wladt
 * @date   Jan 31, 2013 2:34:11 PM
 *
 * @brief
 */

#ifndef __CWT_ERRORABLE_HPP__
#define __CWT_ERRORABLE_HPP__

#include <pfs/string.hpp>
#include <pfs/vector.hpp>

namespace cwt {

class DLL_API errorable
{
	struct erritem
	{
		erritem () : _ntimes(0), _errstr() {}
		erritem (const pfs::string & s) : _ntimes(0), _errstr(s) {}
		int    _ntimes;
		pfs::string _errstr;
	};

	pfs::vector<erritem> _errors;

protected:
	errorable() {}

public:
	virtual ~errorable () { clearErrors(); }
	void addSystemError (int errn, const pfs::string & caption);
	void addError (const pfs::string & text);

	void clearErrors() { _errors.clear(); }
	size_t errorCount() const;
	const pfs::string errorTextAt(size_t index) const;
	int errorRepeatCount(size_t index) const;
	bool isError() const { return _errors.size() != 0; }
	bool isGood() const { return _errors.size() == 0; }
	const pfs::string lastErrorText() const;
};

inline size_t errorable::errorCount() const
{
	return _errors.size();
}

inline const pfs::string errorable::errorTextAt(size_t index) const
{
	return index < _errors.size()
			? pfs::string(_errors[index]._errstr)
			: pfs::string();
}

inline int errorable::errorRepeatCount(size_t index) const
{
	return index < _errors.size()
			? _errors[index]._ntimes
			: -1;
}

inline const pfs::string errorable::lastErrorText() const
{
	return _errors.size() > 0
			? pfs::string(_errors.last()._errstr)
			: pfs::string();
}

inline void errorable::addError(const pfs::string & text)
{
	if (_errors.size() > 0 && _errors.last()._errstr == text) {
		;
	} else {
		_errors.append(erritem(text));
	}
	_errors.last()._ntimes++;
}

} // cwt

#endif /* __CWT_ERRORABLE_HPP__ */

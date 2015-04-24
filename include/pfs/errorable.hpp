/**
 * @file   errorable.hpp
 * @author wladt
 * @date   Jan 31, 2013 2:34:11 PM
 *
 * @brief
 */

#ifndef __PFS_ERRORABLE_HPP__
#define __PFS_ERRORABLE_HPP__

#include <pfs/string.hpp>
#include <pfs/vector.hpp>
#include <ostream>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

class DLL_API errorable
{
protected:
	struct erritem
	{
		erritem () : _ntimes(0), _errstr() {}
		erritem (const pfs::string & s) : _ntimes(0), _errstr(s) {}
		erritem (const erritem & e) : _ntimes(e._ntimes), _errstr(e._errstr) {}
		int    _ntimes;
		pfs::string _errstr;
	};

	pfs::vector<erritem> _errors;

protected:
	errorable () {}
	errorable (const errorable & e) : _errors(e._errors) {}

public:
	virtual ~errorable () { clearErrors(); }
	void addError (const pfs::string & text);
	void addErrors (const errorable & e) { _errors.append(e._errors); }

	void clearErrors () { _errors.clear(); }
	size_t errorCount () const;
	const pfs::string errorTextAt (size_t index) const;
	int errorRepeatCount (size_t index) const;
	bool isError () const { return _errors.size() != 0; }
	bool isGood () const { return _errors.size() == 0; }
	const pfs::string lastErrorText () const;

	void print (std::ostream &);
};


inline size_t errorable::errorCount () const
{
	return _errors.size();
}

inline const pfs::string errorable::errorTextAt (size_t index) const
{
	return index < _errors.size()
			? pfs::string((&_errors[index])->_errstr)
			: pfs::string();
}

inline int errorable::errorRepeatCount (size_t index) const
{
	return index < _errors.size()
			? (&_errors[index])->_ntimes
			: -1;
}

inline const pfs::string errorable::lastErrorText () const
{
	return _errors.size() > 0
			? pfs::string((&_errors.last())->_errstr)
			: pfs::string();
}

inline std::ostream & operator << (std::ostream & out, errorable & ex)
{
   ex.print(out);
   return out;
}


} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_ERRORABLE_HPP__ */

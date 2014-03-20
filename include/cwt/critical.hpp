/**
 * @file   critical.hpp
 * @author wladt
 * @date   Fwb 14, 2014
 *
 * @brief
 */

#ifndef __CWT_CRITICAL_HPP__
#define __CWT_CRITICAL_HPP__

#include <pfs/string.hpp>
#include <pfs/stringlist.hpp>
#include <pfs/map.hpp>
#include <cwt/errorable.hpp>

namespace cwt {

class critical
{
	pfs::string _text;
	pfs::stringlist _detailList;

public:
	critical () : _text(), _detailList() {}

	critical (const pfs::string & text, const pfs::string & detail = pfs::string())
		: _text(text)
		, _detailList()
	{
		if (!detail.isEmpty())
			_detailList.append(detail);
	}

	critical (const pfs::string & text, const pfs::stringlist & details)
		: _text(text)
		, _detailList()
		{ addDetails(details); }

	critical (const pfs::string & text, const cwt::errorable & details)
		: _text(text)
		, _detailList()
		{ addDetails(details); }

	void addDetail (const pfs::string & detail)
		{ _detailList.append(detail); }

	void addDetails (const pfs::stringlist & details)
		{ _detailList.append(details); }

	void addDetails (const cwt::errorable & details);

	pfs::string text () const { return _text; }
	pfs::stringlist details () const { return _detailList; }
};

} // cwt

#endif /* __CWT_CRITICAL_HPP__ */

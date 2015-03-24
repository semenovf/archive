/**
 * @file   critical.hpp
 * @author wladt
 * @date   Fwb 14, 2014
 *
 * @brief
 */

#ifndef __PFS_CRITICAL_HPP__
#define __PFS_CRITICAL_HPP__

#include <pfs/string.hpp>
#include <pfs/stringlist.hpp>
#include <pfs/map.hpp>
#include <pfs/errorable_ext.hpp>

namespace pfs {

class critical
{
	string _text;
	stringlist _detailList;

public:
	critical () : _text(), _detailList() {}

	critical (const string & text, const string & detail = string())
		: _text(text)
		, _detailList()
	{
		if (!detail.isEmpty())
			_detailList.append(detail);
	}

	critical (const string & text, const stringlist & details)
		: _text(text)
		, _detailList()
		{ addDetails(details); }

	critical (const string & text, const errorable_ext & details)
		: _text(text)
		, _detailList()
		{ addDetails(details); }

	void addDetail (const string & detail)
		{ _detailList.append(detail); }

	void addDetails (const stringlist & details)
		{ _detailList.append(details); }

	void addDetails (const errorable_ext & details);

	string text () const { return _text; }
	stringlist details () const { return _detailList; }
};

} // pfs

#endif /* __PFS_CRITICAL_HPP__ */

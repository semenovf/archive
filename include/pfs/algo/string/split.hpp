/*
 * split.hpp
 *
 *  Created on: Oct 21, 2015
 *      Author: wladt
 */

#ifndef __PFS_ALGO_STRING_SPLIT_HPP__
#define __PFS_ALGO_STRING_SPLIT_HPP__

#include <pfs/algo/string/find.hpp>
#include <pfs/bits/iterator/advance.hpp>
#include <pfs/bits/iterator/distance.hpp>

namespace pfs {

enum {
	  DontKeepEmpty = false
	, KeepEmpty = true
};

// Sequence must provide methods:
// 		String::push_back(const value_type &) - appends symbol to sequence
// 		String::empty() - checks for empty
//
template <typename InputIt1, typename InputIt2, typename Sequence>
Sequence & split (
		  InputIt1 itBegin
		, InputIt1 itEnd
		, InputIt2 separatorBegin
		, InputIt2 separatorEnd
		, bool keepEmpty
		, Sequence & result)
{
	typedef typename Sequence::value_type value_type;
	typedef typename InputIt2::difference_type difference_type;

	if (itBegin == itEnd)
		return result;

	difference_type n = pfs::distance(separatorBegin, separatorEnd);

	// "/"

	while (itBegin != itEnd) {
		InputIt1 it = pfs::find(itBegin
				, itEnd
				, separatorBegin
				, separatorEnd);

		if (it == itEnd)
			break;

		value_type v(itBegin, it);

		if (!(v.empty() && !keepEmpty)) {
			result.push_back(v);
		}

		// Skip separator
		//
		pfs::advance(it, n);
		itBegin = it;
	}

	if (itBegin != itEnd)
		result.push_back(value_type(itBegin, itEnd));
	else {
		if (keepEmpty)
			result.push_back(value_type());
	}

	return result;
}

/*
template <typename String, typename Sequence>
void split (
		  String::const_iterator itBegin
		, String::const_iterator itEnd
		, bool isOneSeparatorChar
		, const String & separator
		, bool keepEmpty
		, const String & openQuote
		, const String & closeQuote
		, Sequence & result)
{
	Sequence::value_type::const_iterator it = itBegin;
	size_t separatorLength = separator.length();
	bool quote = false;
	String qopen(openQuote);
	String qclose(closeQuote);

	if (qclose.empty()) {
		if (!qopen.empty())
			qclose= qopen;
	}

	if (qopen.empty()) {
		if (!qclose.empty())
			qopen = qclose;
	}

	while (it != itEnd) {
		if (openQuoteChar != ucchar::Null && *it == openQuoteChar) {
			if (quote)
				quote = false;
			else
				quote = true;
			++it;
		} else if (!quote
				&& ((isOneSeparatorChar && separator.contains(*it))
					|| (!isOneSeparatorChar && this->startsWith(it, separator)))) {

			mbcs_string s(itBegin, it);

			if (!s.isEmpty()) {
				r.append(s);
			} else {
				if (keepEmpty)
					r.append(s);
			}

			it += isOneSeparatorChar ? 1 : separatorLength;
			itBegin = it;
		} else {
			++it;
		}
	}

	if (itBegin != it || itBegin == itEnd) {
		mbcs_string s(itBegin, it);
		if (!s.isEmpty()) {
			r.append(s);
		} else {
			if (keepEmpty)
				r.append(s);
		}
	}

	if (r.isEmpty())
		r.append(*this);


	PFS_VERIFY_X(!quote, "Unbalanced quote");
	if (quote)
		r.clear();

	return r;
}
*/

//stringlist split (const mbcs_string & separator
//		, bool keepEmpty = KeepEmpty
//		, ucchar quoteChar = ucchar(ucchar::Null)) const
//{
//	return split(false, separator, keepEmpty, quoteChar);
//}
//
//stringlist split (const ucchar & separator
//		, bool keepEmpty = KeepEmpty
//		, ucchar quoteChar = ucchar(ucchar::Null)) const
//{
//	return split(false, mbcs_string(1, separator), keepEmpty, quoteChar);
//}
//
//stringlist splitOneOf (const mbcs_string & separators
//		, bool keepEmpty = KeepEmpty
//		, ucchar quoteChar = ucchar(ucchar::Null)) const
//{
//	return split(true, mbcs_string(separators), keepEmpty, quoteChar);
//}

} // pfs

#endif /* __PFS_ALGO_STRING_SPLIT_HPP__ */

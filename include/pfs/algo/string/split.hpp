/*
 * split.hpp
 *
 *  Created on: Oct 21, 2015
 *      Author: wladt
 */

#ifndef __PFS_ALGO_STRING_SPLIT_HPP__
#define __PFS_ALGO_STRING_SPLIT_HPP__

namespace pfs {

enum {
	  DontKeepEmpty = false
	, KeepEmpty = true
};

// Sequence must provide push_back(const String &) methods
//
template <typename Sequence>
void split (
		  Sequence * result
		, Sequence::value_type::const_iterator itBegin
		, Sequence::value_type::const_iterator itEnd
		, bool isOneSeparatorChar
		, const Sequence::value_type separator
		, bool keepEmpty
		, ucchar openQuoteChar
		, ucchar closeQuoteChar)
{
	Sequence::value_type::const_iterator it = itBegin;
	size_t separatorLength = separator.length();
	bool quote = false;
	bool allowQuoting = false;

	if (openQuoteChar != ucchar::Null)
		allowQuoting = true;

	if (allowQuoting && closeQuoteChar == ucchar::Null)
		closeQuoteChar = openQuoteChar;

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

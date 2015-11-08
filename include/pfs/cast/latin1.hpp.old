/*
 * latin1.hpp
 *
 *  Created on: Oct 25, 2015
 *      Author: wladt
 */

#ifndef __PFS_CAST_LATIN1_HPP__
#define __PFS_CAST_LATIN1_HPP__

#include <cstring>

namespace pfs {

struct latin1_cast_state
{
	latin1_cast_state () : invalidChars(0), replacementChar('?') {}
	size_t invalidChars;
	char replacementChar;
};

template <typename InputIterator, typename String>
String & latin1_cast (InputIterator first, InputIterator last, String & result, latin1_cast_state * pstate = 0)
{
	latin1_cast_state state;

	if (pstate)
		state.replacementChar = pstate->replacementChar;

	while (first != last) {
		if (*first < 127) {
			result.push_back(*first);
		} else {
			result.push_back(state.replacementChar);
			++state.invalidChars;
		}
		++first;
	}

	if (pstate)
		pstate->invalidChars += state.invalidChars;

	return result;
}

template <typename String>
String & latin1_cast (const char * latin1, size_t n, String & result, latin1_cast_state * pstate = 0)
{
	return latin1_cast<const char *, String>(latin1, latin1 + n, result, pstate);
}

template <typename String>
inline String & latin1_cast (const char * latin1, String & result, latin1_cast_state * pstate = 0)
{
	return latin1_cast(latin1, strlen(latin1), result, pstate);
}

} // pfs

#endif /* __PFS_CAST_LATIN1_HPP__ */

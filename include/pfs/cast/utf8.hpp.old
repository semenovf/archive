/*
 * utf8.hpp
 *
 *  Created on: Oct 25, 2015
 *      Author: wladt
 */

#ifndef __PFS_CAST_UTF8_HPP__
#define __PFS_CAST_UTF8_HPP__

#include <pfs/iterator/utf8_iterator.hpp>

namespace pfs {

const uint8_t * advance_utf8_char (
          const uint8_t * begin
        , const uint8_t * end
        , uint32_t & min_uc
        , size_t * pnremain
        , size_t * pinvalid
        , ucchar * pch);

struct utf8_cast_state
{
	utf8_cast_state () : nremain(0), invalidChars(0), replacementChar('?') {}
	size_t nremain;
	size_t invalidChars;
	uint32_t replacementChar;
};

template <typename InputIterator, typename String>
String & utf8_cast (InputIterator first
		, InputIterator last
		, String & result
		, utf8_cast_state * pstate = 0)
{
	while (first != last) {
		result.push_back(*first);
		++first;
	}

	return result;
}

template <typename String>
String & utf8_cast (const uint8_t * first
		, const uint8_t * last
		, String & result
		, utf8_cast_state * pstate = 0)
{
    size_t invalidChars = 0;
    size_t nremain = 0;
    uint32_t min_uc = 0; // for 'Overlong' encodings recognition
    uint8_t replacement[6];
    uint8_t encodedChar[6];
    size_t replacementSize = pstate
            ? ucchar(pstate->replacementChar).encode<uint8_t>(replacement, 6)
            : ucchar(ucchar::ReplacementChar).encode<uint8_t>(replacement, 6);

	while (first != last) {
        ucchar ch;

        first = advance_utf8_char (
                  first
                , last
                , min_uc
                , & nremain
                , & invalidChars
                , & ch);

        if (ch.isValid()) {
            // skip the BOM
            if (ch == ucchar::BomChar)
                continue;

            size_t sz = ch.encodeUtf8(encodedChar, 6);
            result->append(encodedChar, sz);
        } else {
            if (nremain > 0) {
                for (size_t j = nremain; j > 0; --j) {
                    result->append(replacement, replacementSize);
                    ++invalidChars;
                }

                first += nremain;
                PFS_ASSERT(first >= last);
            } else {
                result->append(replacement, replacementSize);
                ++first;
            }
        }
    }

    if (pstate) {
        pstate->invalidChars += invalidChars;
        pstate->nremain = nremain;
    }

    return result;
}

template <typename String>
inline String & utf8_cast (const char * first
		, const char * last
		, String & result
		, utf8_cast_state * pstate = 0)
{
	return utf8_cast(static_cast<const char *>(first)
			, static_cast<const char *>(last)
			, result
			, pstate);
}

} // pfs

#endif /* __PFS_CAST_UTF8_HPP__ */

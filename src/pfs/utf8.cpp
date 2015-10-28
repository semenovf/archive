/*
 * utf8.cpp
 *
 *  Created on: 18 апр. 2015 г.
 *      Author: wladt
 */


#include "pfs/ucchar.hpp"

namespace pfs {

/* UTF-8
 *
 * Bits Last code point/    Byte 1      Byte 2      Byte 3      Byte 4      Byte 5      Byte 6
 *  7   U+007F              0xxxxxxx
 *  11  U+07FF              110xxxxx    10xxxxxx
 *  16  U+FFFF              1110xxxx    10xxxxxx    10xxxxxx
 *  21  U+1FFFFF            11110xxx    10xxxxxx    10xxxxxx    10xxxxxx
 *  26  U+3FFFFFF           111110xx    10xxxxxx    10xxxxxx    10xxxxxx    10xxxxxx
 *  31  U+7FFFFFFF          1111110x    10xxxxxx    10xxxxxx    10xxxxxx    10xxxxxx    10xxxxxx
 */

inline uint32_t construct_codepoint (const uint8_t * p
        , size_t n
        , uint32_t initial
        , bool & isInvalid)
{
    uint32_t r = initial;

    isInvalid = false;

    for (size_t i = 1; i < n; ++i) {
        if ((p[i] & 0xC0) == 0x80) {
            r = (r << 6) | (p[i] & 0x3F);
        } else {
            isInvalid = true;
            break;
        }
    }

    return r;
}

/**
 * @note For internal use only. Used for parse untrusted source
 *       as UTF8-encoded sequence.
 *
 * @param begin Pointer to first code unit.
 * @param end Pointer to after last code unit.
 * @param min_uc Minimal Unicode code point for overlong sequence recognition.
 * @param pnremain Pointer to store number of requested code units.
 * @param pinvalid Pointer to store incremented number of invalid chars.
 * @param pch Pointer store resulting unicode char.
 *      Recommended to pass replacement char in *pch.
 * @return If successfully recognized UTF8-encoded char:
 *              - return @c begin pointer incremented by valid code units
 *              - *pnremain stores zero
 *              - *pinvalid unmodified
 *              - *pch stores valid unicode char
 *
 *         If need to request more units (one in this case)
 *         to recognized UTF8-encoded char:
 *              - return @c begin pointer
 *              - *pnremain stores number of requested units (one in this case)
 *              - *pinvalid unmodified
 *              - *pch stores invalid unicode char
 *
 *         If code unit sequence is invalid:
 *              - return @c begin pointer incremented by one (skipped one code unit)
 *              - *pnremain stores zero
 *              - *pinvalid incremented by 1
 *              - *pch stores invalid unicode char
 */
const uint8_t * advance_utf8_char (
          const uint8_t * begin
        , const uint8_t * end
        , uint32_t & min_uc
        , size_t * pnremain
        , size_t * pinvalid
        , ucchar * pch)
{
    PFS_ASSERT(begin < end);

    const uint8_t * p = begin;
    uint32_t uc = 0;
    bool isInvalid = false;
    size_t nremain = 0; // number of requested units

    uint8_t b = *p;
    int need = 0;
    min_uc = 0;

    if (b < 128) {
        uc = b;
        need = 0;
    } else if ((b & 0xE0) == 0xC0) {
        uc = b & 0x1F;
        need = 1;
        min_uc = 0x80;
    } else if ((b & 0xF0) == 0xE0) {
        uc = b & 0x0F;
        need = 2;
        min_uc = 0x800;
    } else if ((b & 0xF8) == 0xF0) {
        uc = b & 0x07;
        need = 3;
        min_uc = 0x10000;
    } else if ((b & 0xFC) == 0xF8) {
        uc = b & 0x03;
        need = 4;
        min_uc = 0x200000;
    } else if ((b & 0xFE) == 0xFC) {
        uc = b & 0x01;
        need = 5;
        min_uc = 0x4000000;
    } else {
        isInvalid = true; // error
    }

    if (!isInvalid) {
        // too few bytes in the buffer
        if (size_t(need) >= size_t(end - begin)) {
            nremain = need - size_t(end - begin) + 1;
        } else {
            uc = construct_codepoint(p, need + 1, uc, isInvalid);

            if (!ucchar::isValid(uc, min_uc))
                isInvalid = true;
        }
    }

    if (!isInvalid) {
        if (nremain) {
            if (pnremain)
                *pnremain = nremain;

            if (pch)
                pch->invalidate();
        } else {
            if (pch)
                *pch = ucchar(uc);
            p += need + 1;
        }
    } else {
        ++p;
        if (pinvalid)
            ++*pinvalid;

        if (pch)
            pch->invalidate();
    }

    return p;
}


#define ASSERT_INVALID_UTF8 PFS_ASSERT_X(false, "invalid (untrusted) UTF8-encoded sequence")

// TODO DEPRECATED
/**
 * @note For internal use only.
 *       Used for forward movement through valid (trusted)
 *       UTF8-encoded sequence.
 *
 * @param begin
 * @param pch
 * @return
 */
const uint8_t * advance_utf8_char (
          const uint8_t * begin
        , ucchar * pch)
{
    const uint8_t * p = begin;

    uint8_t b = *p;
    uint32_t uc = 0;
    int need = 0;

    if (b < 128) {
        uc = b;
        need = 0;
    } else if ((b & 0xE0) == 0xC0) {
        uc = b & 0x1F;
        need = 1;
    } else if ((b & 0xF0) == 0xE0) {
        uc = b & 0x0F;
        need = 2;
    } else if ((b & 0xF8) == 0xF0) {
        uc = b & 0x07;
        need = 3;
    } else if ((b & 0xFC) == 0xF8) {
        uc = b & 0x03;
        need = 4;
    } else if ((b & 0xFE) == 0xFC) {
        uc = b & 0x01;
        need = 5;
    } else {
        ASSERT_INVALID_UTF8;
    }

    bool isInvalid = false;
    uc = construct_codepoint(p, need + 1, uc, isInvalid);

    if (isInvalid) {
        ASSERT_INVALID_UTF8;
    }

    if (pch)
        *pch = ucchar(uc);

    p += need + 1;

    return p;
}



/**
 * @note For internal use only.
 *       Used for backward movement through valid (trusted)
 *       UTF8-encoded sequence.
 *
 * @param rbegin
 * @param pch
 * @return
 */
const uint8_t * backward_utf8_char (
          const uint8_t * rbegin
        , ucchar * pch)
{
    const uint8_t * p = rbegin;

    uint32_t uc = 0;
    int need = 0;

    if ((*(p - 1) & 0x80) == 0x00) {
        uc = uint32_t(*(p - 1));
        need = 0;
        --p;
    } else if ((*(p - 2) & 0xE0) == 0xC0) {
        uc = *(p - 2) & 0x1F;
        need = 1;
        p -= 2;
    } else if ((*(p - 3) & 0xF0) == 0xE0) {
        uc = *(p - 3) & 0x0F;
        need = 2;
        p -= 3;
    } else if ((*(p - 4) & 0xF8) == 0xF0) {
        uc = *(p - 4) & 0x07;
        need = 3;
        p -= 4;
    } else if ((*(p - 5) & 0xFC) == 0xF8) {
        uc = *(p - 5) & 0x03;
        need = 4;
        p -= 5;
    } else if ((*(p - 6) & 0xFE) == 0xFC) {
        uc = *(p - 6) & 0x01;
        need = 5;
        p -= 6;
    } else {
        ASSERT_INVALID_UTF8;
    }

    if (pch) {
        bool isInvalid = false;

        uc = construct_codepoint(p, need + 1, uc, isInvalid);

        if (isInvalid)
            ASSERT_INVALID_UTF8;
    }

    return p;
}


/**
 * @brief Encodes Unicode character to UTF8-encoded sequence.
 *
 * @param utf
 * @return Number of bytes required to store Unicode character encoded to UTF8.
 */
size_t ucchar::encodeUtf8 (uint8_t * buffer, size_t sz)
{
    uint8_t * p = buffer;

    if (_value < 0x80) {
    	PFS_ASSERT_X(sz >= 1, "buffer must be at least 1 unit size");
        *p++ = uint8_t(_value);
    } else if (_value < 0x0800) {
    	PFS_ASSERT_X(sz >= 2, "buffer must be at least 2 units size");
    	*p++ = 0xC0 | uint8_t(_value >> 6);
    	*p++ = 0x80 | uint8_t(_value & 0x3f);
    } else if (_value < 0x10000) {
    	PFS_ASSERT_X(sz >= 3, "buffer must be at least 3 units size");
    	*p++ = 0xE0 | uint8_t(_value >> 12);
    	*p++ = 0x80 | (uint8_t(_value >> 6)  & 0x3F);
    	*p++ = 0x80 | uint8_t(_value & 0x3F);
    } else if (_value < 0x200000) {
    	PFS_ASSERT_X(sz >= 4, "buffer must be at least 4 units size");
    	*p++ = 0xF0 | uint8_t(_value >> 18);
    	*p++ = 0x80 | (uint8_t(_value >> 12) & 0x3F);
    	*p++ = 0x80 | (uint8_t(_value >> 6)  & 0x3F);
    	*p++ = 0x80 | uint8_t(_value & 0x3F);
    } else if (_value < 0x4000000) {
    	PFS_ASSERT_X(sz >= 5, "buffer must be at least 5 units size");
    	*p++ = 0xF8 | uint8_t(_value >> 24);
    	*p++ = 0x80 | (uint8_t(_value >> 18) & 0x3F);
    	*p++ = 0x80 | (uint8_t(_value >> 12) & 0x3F);
    	*p++ = 0x80 | (uint8_t(_value >> 6)  & 0x3F);
    	*p++ = 0x80 | uint8_t(_value & 0x3F);
    } else if (_value < 0x80000000) {
    	PFS_ASSERT_X(sz >= 6, "buffer must be at least 6 units size");
    	*p++ = 0xFC | uint8_t(_value >> 30);
    	*p++ = 0x80 | (uint8_t(_value >> 24) & 0x3F);
    	*p++ = 0x80 | (uint8_t(_value >> 18) & 0x3F);
    	*p++ = 0x80 | (uint8_t(_value >> 12) & 0x3F);
    	*p++ = 0x80 | (uint8_t(_value >> 6)  & 0x3F);
    	*p++ = 0x80 | uint8_t(_value & 0x3F);
    }
    //*p = '\0';

    return size_t(p - buffer);
}


size_t ucchar::decodeUtf8 (const uint8_t * begin, size_t sz)
{
    uint32_t min_uc = 0;
    const uint8_t * end = begin + sz;
    const uint8_t * p = advance_utf8_char(begin, end
            , min_uc
            , nullptr // pnremain
            , nullptr // pinvalid
            , this);

    PFS_ASSERT(p >= begin && p <= end);
    return this->isValid() ? p - begin : 0;
}

} // pfs

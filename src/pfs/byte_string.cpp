/*
 * bytes_string.cpp
 *
 *  Created on: 23 дек. 2014 г.
 *      Author: wladt
 */

#include <pfs/byte_string.hpp>
//#include <pfs/string.hpp>
//#include <pfs/bits/strtointegral.hpp>
//#include <pfs/bits/strtoreal.hpp>
//#include <cstring>

namespace pfs {

byte_string & base64_encode (const byte_string & src, byte_string & result)
{
    static const char alphabet[] = "ABCDEFGH" "IJKLMNOP" "QRSTUVWX" "YZabcdef"
		            "ghijklmn" "opqrstuv" "wxyz0123" "456789+/";
    static const char padchar = '=';
    int padlen = 0;
    size_t sz = src.size();

    result.clear();
    result.reserve((sz * 4) / 3 + 3);

    size_t i = 0;
    byte_string::const_pointer d = src.data();

    while (i < sz) {
		int chunk = 0;
			chunk |= int(d[i++]) << 16;
		if (i == sz) {
			padlen = 2;
		} else {
			chunk |= int(d[i++]) << 8;

			if (i == sz)
				padlen = 1;
			else
				chunk |= int(d[i++]);
		}

		int j = (chunk & 0x00fc0000) >> 18;
		int k = (chunk & 0x0003f000) >> 12;
		int l = (chunk & 0x00000fc0) >> 6;
		int m = (chunk & 0x0000003f);

		result.append(1, alphabet[j]);
		result.append(1, alphabet[k]);

		if (padlen > 1)
			result.append(1, padchar);
		else
			result.append(1, alphabet[l]);
		if (padlen > 0)
			result.append(1, padchar);
		else
			result.append(1, alphabet[m]);
    }

    return result;
}

byte_string & base64_decode (const byte_string & src, byte_string & result)
{
    size_t sz = src.size();

    result.clear();
    result.reserve((sz * 3) / 4);

    unsigned int buf = 0;
    int nbits = 0;

    for (size_t i = 0; i < sz; ++i) {
		int ch = src.at(i);
		int d = -1;

		if (ch >= 'A' && ch <= 'Z')
			d = ch - 'A';
		else if (ch >= 'a' && ch <= 'z')
			d = ch - 'a' + 26;
		else if (ch >= '0' && ch <= '9')
			d = ch - '0' + 52;
		else if (ch == '+')
			d = 62;
		else if (ch == '/')
			d = 63;

		if (d != -1) {
			buf = (buf << 6) | d;
			nbits += 6;

			if (nbits >= 8) {
				nbits -= 8;
				result.push_back(buf >> nbits);
				buf &= (1 << nbits) - 1;
			}
		}
    }

    result.resize(result.size());
    return result;
}

template <>
bool unpack (unpack_context & ctx, byte_string & v)
{
    if (ctx.fail)
        return false;
    
    byte_string::size_type size = 0;
    
    if (unpack(ctx, size)) {
        if (distance(ctx.b, ctx.e) >= size) {
            v = byte_string(ctx.b, ctx.e);
            ctx.fail = false;
            advance(ctx.b, size);
        } else {
            ctx.fail = true;
        }
    }
    
    return not ctx.fail;
}

namespace details {

// see http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html#serialization
//
#if __COMMENT__
byte_string & pack_ieee754 (byte_string & appender
    , real64_t const & v
    , endian const & order
    , unsigned bits
    , unsigned expbits)
{
	real64_t fnorm;
	int shift;
	int64_t sign, exp, significand;
	unsigned significandbits = bits - expbits - 1; // -1 for sign bit

	if (v == real64_t(0.0))
        return pack_integral(appender, uint64_t(0), order);

	// check sign and begin normalization
	if (v < real64_t(0)) {
        sign = 1; 
        fnorm = -v;
    } else { 
        sign = 0;
        fnorm = v; 
    }

	// get the normalized form of f and track the exponent
	shift = 0;
	
    while (fnorm >= real64_t(2.0)) { 
        fnorm /= real64_t(2.0);
        ++shift;
    }
    
	while (fnorm < real64_t(1.0)) {
        fnorm *= real64_t(2.0);
        --shift;
    }
    
	fnorm = fnorm - real64_t(1.0);

	// calculate the binary form (non-float) of the significand data
	significand = fnorm * ((int64_t(1) << significandbits) + real64_t(0.5));

	// get the biased exponent
	exp = shift + ((1 << (expbits - 1)) - 1); // shift + bias

	uint64_t peer = (sign << (bits-1)) | (exp << (bits - expbits - 1)) | significand;
    return pack_integral(appender, peer, order);
}

bool unpack_ieee754 (unpack_context & ctx
        , real64_t & v
        , unsigned bits
        , unsigned expbits)
{
	int64_t shift;
	unsigned bias;
	unsigned significandbits = bits - expbits - 1; // -1 for sign bit

    uint64_t d = 0;
    
    if (not unpack_integral(ctx, d)) {
        return false;
    }
    
	if (d == 0) {
        v = real64_t(0);
        return true;
    }

	// pull the significand
	v = (d & ((int64_t(1) << significandbits) - 1)); // mask
	v /= (int64_t(1) << significandbits); // convert back to float
	v += real64_t(1.0); // add the one back on

	// deal with the exponent
	bias = (1 << (expbits - 1)) - 1;
	shift = ((d >> significandbits) & ((int64_t(1) << expbits) - 1)) - bias;
    
	while (shift > 0) {
        v *= real64_t(2.0);
        --shift; 
    }
    
	while (shift < 0) {
        v /= real64_t(2.0);
        ++shift; 
    }

	// sign it
	v *= (d >> (bits - 1)) & 1 ? real64_t(-1.0): real64_t(1.0);

	return true;
}
#endif

}

} // pfs

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
byte_string & pack (byte_string & appender, byte_string const & v, const endian & order)
{
    appender.reserve(appender.size() + sizeof(byte_string::size_type) + v.size());
    pack(appender, v.size(), order); // pack size of byte_string
    appender.append(v);
    return appender;
}


template <>
inline bool unpack (unpack_context & ctx, byte_string & v)
{
    if (ctx.fail)
        return false;
    
    byte_string::size_type size = 0;
    
    if (unpack(ctx, size)) {
        if (distance(ctx.b, ctx.e) <= size) {
            v = byte_string(ctx.b, ctx.e);
            ctx.fail = false;
            advance(ctx.b, size);
        } else {
            ctx.fail = true;
        }
    }
    
    return not ctx.fail;
}

} // pfs

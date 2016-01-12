/*
 * inet_addr.cpp
 *
 *  Created on: Jun 25, 2015
 *      Author: wladt
 */

#include <netinet/in.h>
#include <pfs/stringlist.hpp>
#include <pfs/utility.hpp>
#include "pfs/net/inet4_addr.hpp"

namespace pfs { namespace net {

inet4_addr::inet4_addr (uint8_t a, uint8_t b, uint8_t c, uint8_t d)
    : _addr(0)
{
    _addr |= (static_cast<uint32_t>(a) << 24);
    _addr |= (static_cast<uint32_t>(b) << 16);
    _addr |= (static_cast<uint32_t>(c) << 8);
    _addr |= static_cast<uint32_t>(d);
}

inet4_addr::inet4_addr (uint8_t a, uint8_t b, uint16_t c)
    : _addr(0)
{
    _addr |= (static_cast<uint32_t>(a) << 24);
    _addr |= (static_cast<uint32_t>(b) << 16);
    _addr |= static_cast<uint32_t>(c);
}

inet4_addr::inet4_addr (uint8_t a, uint32_t b)
    : _addr(0)
{
    _addr |= (static_cast<uint32_t>(a) << 24);
    _addr |= b;
}

inet4_addr::inet4_addr (uint32_t a)
    : _addr(a)
{}

static bool __parse_part (uint32_t & result
		, uint32_t maxvalue
		, string::const_iterator begin
		, string::const_iterator end)
{
	bool ok = true;
	uint32_t r = lexical_cast<uint32_t>(begin, end, 0, & ok);

	if (! ok)
		return false;

	if (!(r <= maxvalue))
		return false;

	result = r;
	return ok;
}


inet4_addr::inet4_addr (const string & s)
	: _addr(invalid_addr_value)
{
	stringlist sl;

	if (s.empty())
		return;

	split(s, _u8("."), true, sl);

	if (sl.size() > 4)
		return;

	switch (sl.size()) {
	case 1: {
		uint32_t A = 0;

		if (__parse_part(A, 0xFFFFFFFF, sl[0].cbegin(), sl[0].cend())) {
			inet4_addr other(A);
			this->swap(other);
		}

		break;
	}

	case 2: {
		uint32_t a = 0;
		uint32_t B = 0;

		if (__parse_part(a, 0xFF, sl[0].cbegin(), sl[0].cend())
				&& __parse_part(B, 0x00FFFFFF, sl[1].cbegin(), sl[1].cend())) {

			inet4_addr other(static_cast<uint8_t>(a), B);
			this->swap(other);
		}

		break;
	}

	case 3: {
		uint32_t a = 0;
		uint32_t b = 0;
		uint32_t C = 0;

		if (__parse_part(a, 0xFF, sl[0].cbegin(), sl[0].cend())
				&& __parse_part(b, 0xFF, sl[1].cbegin(), sl[1].cend())
				&& __parse_part(C, 0x0000FFFF, sl[2].cbegin(), sl[2].cend())) {

			inet4_addr other(static_cast<uint8_t>(a)
					, static_cast<uint8_t>(b)
					, static_cast<uint16_t>(C));
			this->swap(other);
		}

		break;
	}

	case 4: {
		uint32_t a = 0;
		uint32_t b = 0;
		uint32_t c = 0;
		uint32_t d = 0;

		if (__parse_part(a, 0xFF, sl[0].cbegin(), sl[0].cend())
				&& __parse_part(b, 0xFF, sl[1].cbegin(), sl[1].cend())
				&& __parse_part(c, 0xFF, sl[2].cbegin(), sl[2].cend())
				&& __parse_part(d, 0xFF, sl[3].cbegin(), sl[3].cend())) {

			inet4_addr other(static_cast<uint8_t>(a)
					, static_cast<uint8_t>(b)
					, static_cast<uint8_t>(c)
					, static_cast<uint8_t>(d));

			this->swap(other);
		}

		break;
	}

	default:
		break;
	}
}

}} // pfs::net

namespace pfs {

inline void __append_prefix (string & r, const string & a, int base)
{
	if (base == 16) {
		size_t len = a.length();
		r.append("0x");

		if (len == 1)
			r.push_back('0');
	} else if (base == 8) {
		size_t len = a.length();
		r.append("0");

		if (len < 3)
			r.push_back('0');

		if (len < 2)
			r.push_back('0');
	}
}

inline void __append_prefix (string & r, int base)
{
	if (base == 16)
		r.append("0x");
	else if (base == 8)
		r.append("0");
}

string to_string (const net::inet4_addr & addr, const string & format, int base)
{
	static const string __default_format("%a.%b.%c.%d");

	string r;

	if (!addr)
		return string();

	const string * f = 0;

	if (format.empty()) {
		f = & __default_format;
	} else {
		f = & format;
	}

	if (not (base == 10 || base == 8 || base == 16)) {
		base = 10;
	}

	string::const_iterator it = f->cbegin();
	string::const_iterator it_end = f->cend();

	bool uppercase = true;

	uint32_t native = addr.native();
	string A = to_string(native, base, uppercase);
	string B = to_string(0x00FFFFFF & native, base, uppercase);
	string C = to_string(0x0000FFFF & native, base, uppercase);
	string a = to_string(static_cast<uint8_t>(0x000000FF & (native >> 24)), base, uppercase);
	string b = to_string(static_cast<uint8_t>(0x000000FF & (native >> 16)), base, uppercase);
	string c = to_string(static_cast<uint8_t>(0x000000FF & (native >> 8)), base, uppercase);
	string d = to_string(static_cast<uint8_t>(0x000000FF & native), base, uppercase);

	while (it != it_end) {
		if (*it == '%') {
			++it;
			if (it == it_end) {
				r.push_back('%');
				break;
			}

			switch ((*it).value) {
			case 'a': {
				__append_prefix(r, a, base);
				r.append(a);
				break;
			}
			case 'b':
				__append_prefix(r, b, base);
				r.append(b);
				break;
			case 'c':
				__append_prefix(r, c, base);
				r.append(c);
				break;
			case 'd':
				__append_prefix(r, d, base);
				r.append(d);
				break;

			case 'A':
				__append_prefix(r, base);
				r.append(A);
				break;
			case 'B':
				__append_prefix(r, base);
				r.append(B);
				break;
			case 'C':
				__append_prefix(r, base);
				r.append(C);
				break;

			default:
				r.push_back(*it);
				break;
			}
		} else {
			r.push_back(*it);
		}

		++it;
	}

	return r;
}

} // pfs



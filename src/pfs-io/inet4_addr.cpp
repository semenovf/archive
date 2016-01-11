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

const uint32_t inet4_addr::invalid_addr_value = max_value<uint32_t>();

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

inet4_addr::inet4_addr (const string & s)
	: _addr(invalid_addr_value)
{
	stringlist sl;

	if (s.empty())
		return;

	split(s, _u8("."), true, sl);

	if (s.size() > 4)
		return;

	bool ok;

	switch (s.size()) {
	case 1: {
		uint32_t A = lexical_cast<uint32_t>(sl[0].cbegin(), sl[0].cend(), 10, & ok);
		if (! ok) {
			A = lexical_cast<uint32_t>(sl[0].cbegin(), sl[0].cend(), 16, & ok);

			if (! ok) {
				A = lexical_cast<uint32_t>(sl[0].cbegin(), sl[0].cend(), 8, & ok);

				if (! ok)
					return;
			}

			_addr = A;
		}
		break;
	}

	case 2:
	case 3:
	case 4:
	default:
		break;
	}
}

}} // pfs::net

namespace pfs {

string to_string (const net::inet4_addr & addr, const string & format, int base)
{
	static const string __default_format("%a.%b.%c.%d");
	static const string prefix16("0x");
	static const string prefix8("0");
	static const string prefix10("");

	string r;

	if (!addr)
		return string();

	const string * f = 0;
	const string * prefix = 0;

	if (format.empty()) {
		f = & __default_format;
	} else {
		f = & format;
	}

	if (not (base == 10 || base == 8 || base == 16)) {
		base = 10;
	}

	switch (base) {
	case 16:
		prefix = & prefix16;
		break;
	case 8:
		prefix = & prefix8;
		break;
	case 10:
	default:
		prefix = & prefix10;
		break;
	}

	string::const_iterator it = f->cbegin();
	string::const_iterator it_end = f->cend();

	uint32_t native = addr.native();
	string A = to_string(addr.native(), base);
	string B = to_string(0x00FFFFFF & native, base);
	string C = to_string(0x0000FFFF & native, base);
	string a = to_string(static_cast<uint8_t>(0x000000FF & (native)));
	string b = to_string(static_cast<uint8_t>(0x000000FF & (native)));
	string c = to_string(static_cast<uint8_t>(0x000000FF & (native)));
	string d = to_string(static_cast<uint8_t>(0x000000FF & native));

	while (it != it_end) {
		if (*it == '%') {
			++it;
			if (it == it_end) {
				r.push_back('%');
				break;
			}

			switch ((*it).value) {
			case 'a':
				r.append(*prefix);
				if (a.size() == 1 and base != 10) // in any UTF encoding size of string in code units will be equals to size in code points
					r.push_back('0');
				r.append(a);
				break;
			case 'b':
				r.append(*prefix);
				if (b.size() == 1 and base != 10) // in any UTF encoding size of string in code units will be equals to size in code points
					r.push_back('0');
				r.append(b);
				break;
			case 'c':
				r.append(*prefix);
				if (c.size() == 1 and base != 10) // in any UTF encoding size of string in code units will be equals to size in code points
					r.push_back('0');
				r.append(c);
				break;
			case 'd':
				r.append(*prefix);
				if (c.size() == 1 and base != 10) // in any UTF encoding size of string in code units will be equals to size in code points
					r.push_back('0');
				r.append(c);
				break;

			case 'A':
				r.append(*prefix);
				r.append(A);
				break;
			case 'B':
				r.append(*prefix);
				r.append(B);
				break;
			case 'C':
				r.append(*prefix);
				r.append(C);
				break;

			default:
				r.push_back(*it);
				break;
			}
		} else {
			r.push_back(*it);
		}
	}

	return r;
}

} // pfs



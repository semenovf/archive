/**
 * @file   serial.cpp
 * @author wladt
 * @date   Apr 11, 2013 9:28:52 AM
 *
 * @brief
 */


#include "pfs/io/serial.hpp"

namespace pfs { namespace io {

uint32_t serial::baudrate () const
{
	uint32_t r;
	settings(& r, nullptr, nullptr, nullptr);
	return r;
}

serial::databits_t serial::databits () const
{
	databits_t r;
	settings(nullptr, & r, nullptr, nullptr);
	return r;
}

serial::stopbits_t serial::stopbits () const
{
	stopbits_t r;
	settings(nullptr, nullptr, & r, nullptr);
	return r;
}

serial::parity_t serial::parity () const
{
	parity_t r;
	settings(nullptr, nullptr, nullptr, & r);
	return r;
}

bool serial::setFormat (format_t fmt)
{
	bool r = true;

	switch(fmt) {
	case serial::Format7E1:
		r &= setDatabits(serial::DB7);
		r &= setStopbits(serial::SB1);
		r &= setParity(serial::ParityEven);
		break;
	case serial::Format7O1:
		r &= setDatabits(serial::DB7);
		r &= setStopbits(serial::SB1);
		r &= setParity(serial::ParityOdd);
		break;
	case serial::Format8N1:
	default:
		r &= setDatabits(serial::DB8);
		r &= setStopbits(serial::SB1);
		r &= setParity(serial::ParityNone);
		break;
	}

	return r;
}

bool serial::setFormat (const pfs::string & fmt)
{
	bool r = true;

	if (fmt.length() != 3) {
		addError(string()
		        << _u8("bad format string")
		        << ":\"" << fmt << "\"");
		return false;
	}

	if (r) {
		ucchar ch = fmt.charAt(0);

		if (ch == pfs::ucchar('5'))
			r &= setDatabits(serial::DB5);
		else if (ch == pfs::ucchar('6'))
			r &= setDatabits(serial::DB6);
		else if (ch == pfs::ucchar('7'))
			r &= setDatabits(serial::DB7);
		else if (ch == pfs::ucchar('8'))
			r &= setDatabits(serial::DB8);
		else {
			addError(string()
			        << _u8("bad data bits value in string format")
			        << ": \"" << fmt << "\"");
			return false;
		}
	}

	if (r) {
		pfs::ucchar ch = fmt.charAt(1);

		if (ch == pfs::ucchar('N'))
			r &= setParity(serial::ParityNone);
		else if (ch == pfs::ucchar('O'))
			r &= setParity(serial::ParityOdd);
		else if (ch == pfs::ucchar('E'))
			r &= setParity(serial::ParityEven);
		else {
			addError(string()
			        << _u8("bad parity value in string format")
			        << ": " << fmt << "\"");
			return false;
		}
	}

	if (r) {
		pfs::ucchar ch = fmt.charAt(2);

		if (ch == pfs::ucchar('1'))
			r &= setStopbits(serial::SB1);
		else if (ch == pfs::ucchar('2'))
			r &= setStopbits(serial::SB2);
		else {
			addError(string()
			        << _u8("bad stop bits value in string format")
			        << ": " << fmt << "\'");
			return false;
		}
	}

	return r;
}


pfs::string serial::parityToString (parity_t parity)
{
	return parity == serial::ParityNone
			? _l1("none")
			: parity == serial::ParityOdd
			  ? _l1("odd")
			  : parity == serial::ParityEven
			    ? _l1("even")
			    : _l1("");
}

pfs::string serial::toString () const
{
	uint32_t   br = 0;
	databits_t db = serial::DB8;
	stopbits_t sb = serial::SB1;
	parity_t   parity = serial::ParityNone;

	settings(& br, & db, & sb, & parity);

	return pfs::string() << "name=" << name()
	        << "; path=" << path()
	        << "; br=" << string::toString(br)
	        << "; db=" << string::toString(db)
	        << "; sb=" << string::toString(sb)
	        << "; parity=" << parityToString(parity);
}

}} // pfs::io


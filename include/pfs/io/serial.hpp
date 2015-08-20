/*
 * serial.hpp
 *
 *  Created on: Oct 23, 2008
 *  Renamed on: Apr 10, 2013
 *
 *  Author: wladt
 */

#ifndef __PFS_IO_SERIAL_HPP__
#define __PFS_IO_SERIAL_HPP__

#include <pfs/string.hpp>
#include <pfs/io/device.hpp>

namespace pfs { namespace io {

class DLL_API serial : public device
{
public:
	enum baudrate_t {
		  BaudrateUnknown = -1
        , BR1200 = 1200
        , BR2400 = 2400
        , BR4800 = 4800
        , BR9600 = 9600
        , BR19200 = 19200
        , BR38400 = 38400
        , BR57600 = 57600
        , BR115200 = 115200
		, BR576000 = 576000
		, BR921600 = 921600
	};

	enum databits_t { DB5 = 5, DB6, DB7, DB8 };
	enum stopbits_t { SB1 = 1, SB2 };
	enum parity_t   { ParityNone, ParityOdd, ParityEven };

	enum flowcontrol_t {
		  NoFlowControl = 0
		, SoftwareFlowControl = 0x0004
		, HardwareFlowControl = 0x0008
	};

	enum format_t {
		  Format8N1
		, Format7E1
		, Format7O1
	};

public:
	serial () : device() {}
	virtual ~serial () {}

	bool open (const pfs::string & path, int32_t oflags);
	bool open (const pfs::string & path, int32_t oflags
			, int baudrate
			, int databits
			, int stopbits
			, parity_t parity
			, int flowControl = NoFlowControl);

	pfs::string name () const;
	pfs::string path () const;

	void settings (uint32_t * br, serial::databits_t * db, serial::stopbits_t * sb, serial::parity_t * parity) const;
	pfs::string toString () const;
//	pfs::string toStringDump ();

	uint32_t   baudrate () const;
	databits_t databits () const;
	stopbits_t stopbits () const;
	parity_t   parity   () const;

    bool setBaudrate    (uint32_t br);
    bool setDatabits    (int);
    bool setStopbits    (int);
    bool setParity      (int);
    bool setFlowControl (int);
    bool setFormat      (format_t);
    bool setFormat      (const pfs::string & format);

public:
    static pfs::string parityToString (parity_t);
};

}} // pfs::io

#endif /* __PFS_SERIAL_HPP__ */

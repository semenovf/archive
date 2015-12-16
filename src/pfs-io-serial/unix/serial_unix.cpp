/*
 * serial_unix.c
 *
 * Created on: Oct 22, 2010
 * Renamed on: Apr 10, 2013
 *
 * Author: wladt
 */

#include <sys/ioctl.h>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <pfs/map.hpp>
#include <pfs/fs.hpp>
#include <pfs/mt.hpp>
#include "pfs/io/serial.hpp"

namespace pfs { namespace io {

static speed_t  to_native_baudrate (uint32_t br);

struct serial_impl : public device_impl
{
	int            _fd;
	string         _path;
	struct termios _currentTermios;
	struct termios _savedTermios;

	serial_impl() : _fd(-1) {}

	bool updateTermios ()
	{
		PFS_ASSERT(_fd >= 0);
		return ::tcsetattr(_fd, TCSANOW, & _currentTermios) < 0
				? false
				: true;
	}

    virtual ssize_t readBytes      (byte_t bytes[], size_t n, errorable_ext & ex);
    virtual ssize_t writeBytes     (const byte_t bytes[], size_t n, errorable_ext & ex);
    virtual size_t  bytes_available () const;
    virtual bool    close    (errorable_ext & ex);
    virtual bool    opened () const;
    virtual void    flush    ();

	bool openSerial (const string & path, int32_t oflags, errorable_ext & ex);
	bool setBaudrate (uint32_t br, errorable_ext & ex);
	bool setDatabits (int db, errorable_ext & ex);
	bool setStopbits (int sb, errorable_ext & ex);
	bool setParity   (int parity, errorable_ext & ex);
	bool setFlowControl (int fc, errorable_ext & ex);
};

ssize_t serial_impl::readBytes (byte_t bytes[], size_t n, errorable_ext & ex)
{
    PFS_ASSERT(_fd  >= 0);

    ssize_t sz = ::read(_fd, bytes, n);
    if (sz < 0) {
//      if (errno != EAGAIN) { // TODO May be bad idea
            ex.addSystemError(errno
                    , string() << _path << ": " << _u8("read failure"));
//      } else {
            sz = 0;
//      }
    }
    return sz;
}

ssize_t serial_impl::writeBytes (const byte_t bytes[], size_t n, errorable_ext & ex)
{
    PFS_ASSERT(_fd  >= 0);
    ssize_t sz = ::write(_fd, bytes, n);

    if( sz < 0 ) {
        ex.addSystemError(errno
                , string() << _path << ": " << _u8("write failure"));
    }
    return sz;
}

size_t serial_impl::bytes_available () const
{
    PFS_ASSERT(_fd  >= 0);
    size_t nbytes;

    PFS_VERIFY(ioctl(_fd, FIONREAD, (char *) & nbytes) >= 0);
    return nbytes;

    return 0;
}

bool serial_impl::close (errorable_ext & ex)
{
    bool r = true;

    if (_fd > 0 ) {
        tcdrain(_fd);
        ::tcsetattr(_fd, TCSANOW, & _savedTermios);
        if (::close(_fd) < 0) {
            ex.addSystemError(errno
                    , string() << _path << ": " << _Tr("close failure"));
            r = false;
        }
    }

    _fd = -1;
    _path.clear();
    return r;
}

bool serial_impl::opened () const
{
    return _fd >= 0;
}

void serial_impl::flush ()
{
    PFS_ASSERT(_fd  >= 0);
    ::tcdrain(_fd);
}


bool serial_impl::openSerial (const string & path, int32_t oflags, errorable_ext & ex)
{
	int native_oflags = 0;

	if (oflags & device::ReadWrite)
		native_oflags |= O_RDWR;
	else if (oflags & device::WriteOnly)
		native_oflags |= O_WRONLY;
	else
		native_oflags |= O_RDONLY;

	if (oflags & device::NonBlocking)
		native_oflags |= O_NONBLOCK;

	native_oflags |= O_NOCTTY; /* because addr refers to a terminal device (see open(2)) */

	_fd = ::open(path.toUtf8().c_str(), native_oflags);

	if (_fd < 0) {
		ex.addSystemError(errno
		        , string() << path << ": " <<  _u8("failed to open"));
		return false;
	}

	::tcflush(_fd, TCIOFLUSH);
	memset(& _savedTermios, 0, sizeof(struct termios));

	/* read port parameters */
	if (::tcgetattr(_fd, & _savedTermios) < 0) {
		ex.addSystemError(errno
		        , string() << path << ": "
		            << _u8("failed to read initial serial port parameters"));
		::close(_fd);
		_fd = -1;
		return false;
	}

	memcpy(& _currentTermios, & _savedTermios, sizeof(struct termios));

	if( (native_oflags & 0xFF) == O_RDONLY || (native_oflags & 0xFF) == O_RDWR )
		_currentTermios.c_cflag |= CREAD; /* enable receiver */
	else
		_currentTermios.c_cflag &= ~CREAD;

	//	/* Modem */
	//	if (opts->mode & SP_MODEM) {
			/* CLOCAL- Ignore modem control lines */
	//		term.c_cflag &= ~CLOCAL;
	//	} else {
	_currentTermios.c_cflag |= CLOCAL; // Ignore modem control lines
	//	}


	_currentTermios.c_cc[VTIME] = 1; // 0
	_currentTermios.c_cc[VMIN] = 60; // 0

	//	/* Canonical mode */
	//	if (opts->mode & SP_CANONICAL) {
	//		term.c_lflag |= ICANON;
	//		term.c_lflag |= (ECHO | ECHOE);
	//		term.c_oflag |= OPOST;
	//	} else {
		/* Raw mode */
	_currentTermios.c_lflag &= ~ICANON;
	_currentTermios.c_lflag &= ~(ECHO | ECHONL | ISIG | IEXTEN);
	_currentTermios.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	_currentTermios.c_oflag &= ~OPOST;
	//	}


	_path = path;
	return true;
}

bool serial_impl::setBaudrate (uint32_t br, errorable_ext & ex)
{
	speed_t speed = to_native_baudrate(br);
	pfs::string errstr;

	if (speed == speed_t(0)) {
		ex.addError(string() << _u8("invalid baud rate value") << ": " << string::toString(br));
		return false;
	}

	if (::cfsetispeed(& _currentTermios, speed ) < 0) {
		ex.addSystemError(errno
		        , string() << _path << ": "
		            << _u8("failed to set input baud rate") << ": " << string::toString(br));
		return false;
	}

	if (::cfsetospeed(& _currentTermios, speed) < 0) {
		ex.addSystemError(errno
		        , string() << _path << ": "
		            << _u8("failed to set output baud rate") << ": " << string::toString(br));
		return false;
	}

	return true;
}

bool serial_impl::setDatabits (int db, errorable_ext & ex)
{
	_currentTermios.c_cflag &= ~CSIZE;

	switch (db) {
	case serial::DB5:
		_currentTermios.c_cflag |= CS5;
		break;
	case serial::DB6:
		_currentTermios.c_cflag |= CS6;
		break;
	case serial::DB7:
		_currentTermios.c_cflag |= CS7;
		break;
	case serial::DB8:
		_currentTermios.c_cflag |= CS8;
		break;
	default:
		ex.addError(string() << _u8("invalid value for data bits") << ": " << string::toString(db));
		return false;
	}

	return true;

#ifdef __COMMENT__

	switch (databits) {
	case 5:
		_currentTermios.c_cflag = (_currentTermios.c_cflag & ~CSIZE) | CS5;
		break;
	case 6:
		_currentTermios.c_cflag = (_currentTermios.c_cflag & ~CSIZE) | CS6;
		break;
	case 7:
		_currentTermios.c_cflag = (_currentTermios.c_cflag & ~CSIZE) | CS7;
		break;
	case 8:
	default:
		_currentTermios.c_cflag = (_currentTermios.c_cflag & ~CSIZE) | CS8;
		break;
	}
#endif
}

bool serial_impl::setStopbits (int sb, errorable_ext & ex)
{
	switch (sb) {
	case serial::SB2:
		_currentTermios.c_cflag |= CSTOPB;
		break;
	case serial::SB1:
		_currentTermios.c_cflag &= ~CSTOPB;
		break;
	default:
		ex.addError(string() << _u8("invalid value for stop bits") << ": " << string::toString(sb));
		return false;
	}

	return true;
}

bool serial_impl::setParity (int parity, errorable_ext & ex)
{
    _currentTermios.c_iflag &= ~(PARMRK | INPCK);
    _currentTermios.c_iflag |= IGNPAR;

	switch (parity) {
	case serial::ParityNone:
		_currentTermios.c_cflag &= ~PARENB;
		break;
	case serial::ParityEven:
		_currentTermios.c_cflag &= ~PARODD;
		_currentTermios.c_cflag |= PARENB;
		break;
	case serial::ParityOdd:
		_currentTermios.c_cflag |= PARENB;
		_currentTermios.c_cflag |= PARODD;
		break;
	default:
		ex.addError(_u8("invalid value for parity"));
//		d._currentTermios.c_cflag |= PARENB;
//		d._currentTermios.c_iflag |= PARMRK | INPCK;
//		d._currentTermios.c_iflag &= ~IGNPAR;
		return false;
	}

	return true;

#ifdef __COMMENT__
	// Parity
	_currentTermios.c_cflag &= ~(PARENB | PARODD);

	if (parity == ParityEven) {
		_currentTermios.c_cflag |= PARENB;
	} else if (parity == ParityOdd) {
		_currentTermios.c_cflag |= (PARENB | PARODD);
	}

#endif
}


bool serial_impl::setFlowControl (int fc, errorable_ext & /*ex*/)
{
	_currentTermios.c_cflag &= ~CRTSCTS;
	_currentTermios.c_iflag &= ~(IXON | IXOFF | IXANY);

	if (fc & serial::HardwareFlowControl) {
    	_currentTermios.c_cflag |= CRTSCTS;
//    	_currentTermios.c_iflag &= ~(IXON | IXOFF | IXANY);
	}

	if (fc & serial::SoftwareFlowControl) {
//	  	_currentTermios.c_cflag &= ~CRTSCTS;
	   	_currentTermios.c_iflag |= IXON | IXOFF | IXANY;
	}

    return true;
}


bool serial::open (const pfs::string & path, int32_t oflags)
{
	return this->open(path, oflags, 9600, 8, 1, ParityNone, NoFlowControl);
}

bool serial::open (const pfs::string & path, int32_t oflags
		, int baudrate
		, int databits
		, int stopbits
		, serial::parity_t parity
		, int flowControl)
{

    if (!_d.isNull()) {
        close();
    } else {
        serial_impl * p = new serial_impl;
        pimpl d(p);
        _d.swap(d);
    }

	serial_impl * d = _d.cast<serial_impl>();

	if (d->openSerial(path, oflags, *this)
			&& d->setBaudrate(baudrate, *this)
			&& d->setDatabits(databits, *this)
			&& d->setStopbits(stopbits, *this)
			&& d->setParity(parity, *this)
			&& d->setFlowControl(flowControl, *this)
			&& d->updateTermios()) {
		return true;
	}

	d->close(*this);
	return false;

#ifdef __COMMENT__
	d._currentTermios.c_cflag |= CLOCAL | CREAD;

	d._currentTermios.c_cflag &= ~CRTSCTS; // flow control

	//   d._currentTermios.c_iflag=IGNPAR | IGNBRK;
	d._currentTermios.c_iflag = IGNBRK;
	//   d._currentTermios.c_iflag=IGNPAR;


	d._currentTermios.c_lflag = 0;
	d._currentTermios.c_oflag = 0;


	if (::tcsetattr(fd, TCSANOW, & d._currentTermios) != 0 ) {
		std::cerr<<"tcsetattr() 1 failed"<<std::endl;
		d.closeFile();
		return false;
	}

	int mcs = 0;
	ioctl(d._fd, TIOCMGET, & mcs);
	mcs |= TIOCM_RTS;
	ioctl(d._fd, TIOCMSET, & mcs);

	if (tcgetattr(d._fd, & d._currentTermios)!=0) {
		std::cerr<<"tcgetattr() 4 failed"<<std::endl;
		d.closeFile();
		return false;
	}

	if (tcsetattr(d._fd, TCSANOW, & d._currentTermios) !=0 ) {
		std::cerr<<"tcsetattr() 2 failed"<<std::endl;
		d.closeFile();
		return false;
	}

	d._path = path;

	return true;
#endif
}


pfs::string serial::name () const
{
	pfs::fs fs;
	return fs.basename(_d.cast<serial_impl>()->_path);
}

pfs::string serial::path () const
{
	return _d.cast<serial_impl>()->_path;
}

static void get_baudrate_mappings (pfs::map<uint32_t, speed_t> ** absoluteNativeMapping
		, pfs::map<speed_t, uint32_t> ** nativeAbsoluteMapping)
{
	static pfs::map<uint32_t, speed_t> __absoluteNativeMapping;
	static pfs::map<speed_t, uint32_t> __nativeAbsoluteMapping;

	static mutex mx;
	pfs::auto_lock<> locker(& mx);

	if (absoluteNativeMapping)
		*absoluteNativeMapping = & __absoluteNativeMapping;

	if (nativeAbsoluteMapping)
		*nativeAbsoluteMapping = & __nativeAbsoluteMapping;

//#ifdef 	__INSERT_VALUE
//#	undef __INSERT_VALUE
//#endif
#define __INSERT_VALUE(br,speed)                                  \
	__absoluteNativeMapping.insert(uint32_t(br), speed_t(speed)); \
	__nativeAbsoluteMapping.insert(speed_t(speed), uint32_t(br))

	// First initialization
	if (__absoluteNativeMapping.size() == 0) {
#ifdef B50
	__INSERT_VALUE(50, B50);
#endif
#ifdef B75
	__INSERT_VALUE(75, B75);
#endif
#ifdef B110
	__INSERT_VALUE(110, B110);
#endif
#ifdef B134
	__INSERT_VALUE(134, B134);
#endif
#ifdef B150
	__INSERT_VALUE(150, B150);
#endif
#ifdef B200
	__INSERT_VALUE(200, B200);
#endif
#ifdef B300
	__INSERT_VALUE(300, B300);
#endif
#ifdef B600
	__INSERT_VALUE(600, B600);
#endif
#ifdef B1200
	__INSERT_VALUE(1200, B1200);
#endif
#ifdef B1800
	__INSERT_VALUE(1800, B1800);
#endif
#ifdef B2400
	__INSERT_VALUE(2400, B2400);
#endif
#ifdef B4800
	__INSERT_VALUE(4800, B4800);
#endif
#ifdef B9600
	__INSERT_VALUE(9600, B9600);
#endif
#ifdef B19200
	__INSERT_VALUE(19200, B19200);
#endif
#ifdef B38400
	__INSERT_VALUE(38400, B38400);
#endif
#ifdef B57600
	__INSERT_VALUE(57600, B57600);
#endif
#ifdef B115200
	__INSERT_VALUE(115200, B115200);
#endif
#ifdef B230400
	__INSERT_VALUE(230400, B230400);
#endif
#ifdef B460800
	__INSERT_VALUE(460800, B460800);
#endif
#ifdef B500000
	__INSERT_VALUE(500000, B500000);
#endif
#ifdef B576000
	__INSERT_VALUE(576000, B576000);
#endif
#ifdef B921600
	__INSERT_VALUE(921600, B921600);
#endif
#ifdef B1000000
	__INSERT_VALUE(1000000, B1000000);
#endif
#ifdef B1152000
	__INSERT_VALUE(1152000, B1152000);
#endif
#ifdef B1500000
	__INSERT_VALUE(1500000, B1500000);
#endif
#ifdef B2000000
	__INSERT_VALUE(2000000, B2000000);
#endif
#ifdef B2500000
	__INSERT_VALUE(2500000, B2500000);
#endif
#ifdef B3000000
	__INSERT_VALUE(3000000, B3000000);
#endif
#ifdef B3500000
	__INSERT_VALUE(3500000, B3500000);
#endif
#ifdef B4000000
	__INSERT_VALUE(4000000, B4000000);
#endif
	}
}

static speed_t  to_native_baudrate (uint32_t br)
{
	pfs::map<uint32_t, speed_t> * absoluteNativeMapping = nullptr;

	get_baudrate_mappings(& absoluteNativeMapping, nullptr);
	PFS_ASSERT(absoluteNativeMapping);
	return absoluteNativeMapping->valueAt(br, speed_t(0));
}

static uint32_t to_absolute_baudrate (speed_t speed)
{
	pfs::map<speed_t, uint32_t> * nativeAbsoluteMapping = nullptr;

	get_baudrate_mappings(nullptr, & nativeAbsoluteMapping);
	PFS_ASSERT(nativeAbsoluteMapping);
	return nativeAbsoluteMapping->valueAt(speed, uint32_t(0));
}

bool serial::setBaudrate (uint32_t br)
{
	serial_impl * d = _d.cast<serial_impl>();

	return d->setBaudrate(br, *this)
			&& d->updateTermios();

	//addSystemError(errno, _Fr("%s: failed to update serial port while setting baud rate") % name());
}

bool serial::setDatabits (int db)
{
	serial_impl * d = _d.cast<serial_impl>();

	return d->setDatabits(db, *this)
			&& d->updateTermios();

//	addSystemError(errno, _Fr("%s: failed to set data bits") % name());
}

bool serial::setStopbits (int sb)
{
	serial_impl * d = _d.cast<serial_impl>();

	return d->setStopbits(sb, *this)
			&& d->updateTermios();

// addSystemError(errno, _Fr("%s: failed to set stop bits") % name());
}

bool serial::setParity (int parity)
{
	serial_impl * d = _d.cast<serial_impl>();

	return d->setParity(parity, *this)
			&& d->updateTermios();

	//addSystemError(errno, _Fr("%s: failed to set parity") % name());
}

bool serial::setFlowControl (int fc)
{
	serial_impl * d = _d.cast<serial_impl>();

	return d->setFlowControl(fc, *this)
			&& d->updateTermios();
}

void serial::settings (uint32_t * br
		, databits_t * db
		, stopbits_t * sb
		, parity_t * parity) const
{
	const serial_impl * d = _d.cast<serial_impl>();
	struct termios term;

	if (d->_fd < 0)
	    return;

	::memset(& term, 0, sizeof(struct termios));

	// Failed to fetch serial port parameters
	PFS_ASSERT_X(::tcgetattr(d->_fd, & term) >= 0, "failed to fetch serial port parameters");

	/* baud rate */
	if (br) {
		*br = to_absolute_baudrate(::cfgetispeed(& term));
		PFS_ASSERT_X(*br != 0, "unrecognized value for baud rate (internal error: no mapping found)");
	}

	/* data bits */
	if (db) {
		tcflag_t cflag = term.c_cflag;
		cflag &= CSIZE;

			 if ((cflag & CS8) == CS8) *db = serial::DB8;
		else if ((cflag & CS7) == CS7) *db = serial::DB7;
		else if ((cflag & CS6) == CS6) *db = serial::DB6;
		else *db = DB5;
	}

	/* stop bits */
	if (sb) {
		if( term.c_cflag & CSTOPB ) *sb = serial::SB2;
		else                        *sb = serial::SB1;
	}

	if (parity) {
		if ((term.c_cflag & PARENB) && (term.c_cflag & PARODD)) *parity = serial::ParityOdd;
		else if (term.c_cflag & PARENB)                         *parity = serial::ParityEven;
		else                                                    *parity = serial::ParityNone;
	}

}

#ifdef __COMMENT__

inline String __yesno(bool y)
{
	return y ? _u8("yes") : _u8("no");
}


String SerialPort::Impl::toStringDump()
{
	struct termios term;
	String parity_str;
	uint32_t br;
	SerialPort::Databits db;
	SerialPort::Stopbits sb;
	SerialPort::Parity parity;

	if (!getInfo(&br, &db, &sb, &parity)) {
		return String();
	}

	parity_str = SerialPort::parityToString(parity);

	::memset(&term, 0, sizeof(struct termios));
	::tcgetattr(m_fd, &term);
	return _Fr("Baud rate: %d\n"
		"Databits : %d\n"
	    "Stopbits : %d\n"
		"Parity   : %s\n"
		"Input modes (only in POSIX):\n"
		"\t[IGNBRK]: %s (Ignore BREAK condition on input)\n"
		"\t[BRKINT]: %s\n"
		"\t[IGNPAR]: %s (Ignore framing errors and parity errors)\n"
		"\t[PARMRK]: %s\n"
		"\t[INPCK] : %s (Enable input parity checking)\n"
		"\t[ISTRIP]: %s (Strip off eighth bit)\n"
		"\t[INLCR] : %s (Translate NL to CR on input)\n"
		"\t[IGNCR] : %s (Ignore carriage return on input)\n"
		"\t[ICRNL] : %s (Translate carriage return to newline on input)\n"
		"\t[IXON]  : %s (Enable XON/XOFF flow control on output)\n"
		"\t[IXANY] : %s (Typing any character will restart stopped output)\n"
		"\t[IXOFF] : %s (Enable XON/XOFF flow control on input)\n"
		"Output modes (only in POSIX):\n"
		"\t[OPOST] : %s (Enable implementation-defined output processing)\n"
		"\t[ONLCR] : %s (Map NL to CR-NL on output)\n"
		"\t[OCRNL] : %s (Map CR to NL on output)\n"
		"\t[ONOCR] : %s (Don’t output CR at column 0)\n"
		"\t[ONLRET]: %s (Don’t output CR)\n"
		"\t[OFILL] : %s (Send fill characters for a delay, rather than using a timed delay)\n"
		"Control modes (only in POSIX):\n"
		"\t[CSIZE] : %d bits (Character size)\n"
		"\t[CSTOPB]: %d bit(s) (Stop bits)\n"
		"\t[CREAD] : %s (Enable receiver)\n"
		"\t[PARENB]: %s (Enable parity generation on output and parity checking for input)\n"
		"\t[PARODD]: %s\n"
		"\t[HUPCL] : %s (Lower modem control lines after hang up)\n"
		"\t[CLOCAL]: %s (Ignore modem control lines)\n"
		"Local modes (only in POSIX):\n"
		"\t[ISIG]  : %s (Generate the signal)\n"
		"\t[ICANON]: %s (Enable canonical mode)\n"
		"\t[ECHO]  : %s (Echo input characters)\n"
		"\t[ECHOE] : %s\n"
		"\t[ECHOK] : %s\n"
		"\t[ECHONL]: %s\n"
		"\t[NOFLSH]: %s (Disable flushing the input and output queues by signals)\n"
		"\t[TOSTOP]: %s (Send the SIGTTOU signal...)\n"
		"\t[IEXTEN]: %s (Enable implementation-defined input processing)\n"
		"Control chars (only in POSIX):\n"
		"\t[VINTR] : 0x%02X (Interrupt character)\n"
		"\t[VQUIT] : 0x%02X (Quit character)\n"
		"\t[VERASE]: 0x%02X (Erase character)\n"
		"\t[VKILL] : 0x%02X (Kill character)\n"
		"\t[VEOF]  : 0x%02X (End-of-file character)\n"
		"\t[VMIN]  : %d (Minimum number of characters for non-canonical read)\n"
		"\t[VEOL]  : 0x%02X (Additional end-of-line character)\n"
		"\t[VTIME] : 0.%d seconds (Timeout for non-canonical read)\n"
		"\t[VSTOP] : 0x%02X (Stop character)\n"
		"\t[VSUSP] : 0x%02X (Suspend character)\n")
		% br
		% int(db)
		% int(sb)
		% parity_str
		% __yesno(term.c_iflag & IGNBRK)
		% __yesno(term.c_iflag & BRKINT)
		% __yesno(term.c_iflag & IGNPAR)
		% __yesno(term.c_iflag & PARMRK)
		% __yesno(term.c_iflag & INPCK)
		% __yesno(term.c_iflag & ISTRIP)
		% __yesno(term.c_iflag & INLCR)
		% __yesno(term.c_iflag & IGNCR)
		% __yesno(term.c_iflag & ICRNL)
		% __yesno(term.c_iflag & IXON)
		% __yesno(term.c_iflag & IXANY)
		% __yesno(term.c_iflag & IXOFF)
		% __yesno(term.c_oflag & OPOST)
		% __yesno(term.c_oflag & ONLCR)
		% __yesno(term.c_oflag & OCRNL)
		% __yesno(term.c_oflag & ONOCR)
		% __yesno(term.c_oflag & ONLRET)
		% __yesno(term.c_oflag & OFILL)
		% (int)db
		% (int)sb
		% __yesno(term.c_cflag & CREAD)
		% __yesno(term.c_cflag & PARENB)
		% __yesno(term.c_cflag & PARODD)
		% __yesno(term.c_cflag & HUPCL)
		% __yesno(term.c_cflag & CLOCAL)
		% __yesno(term.c_lflag & ISIG)
		% __yesno(term.c_lflag & ICANON)
		% __yesno(term.c_lflag & ECHO)
		% __yesno(term.c_lflag & ECHOE)
		% __yesno(term.c_lflag & ECHOK)
		% __yesno(term.c_lflag & ECHONL)
		% __yesno(term.c_lflag & NOFLSH)
		% __yesno(term.c_lflag & TOSTOP)
		% __yesno(term.c_lflag & IEXTEN)
		% (int)term.c_cc[VINTR]
		% (int)term.c_cc[VQUIT]
		% (int)term.c_cc[VERASE]
		% (int)term.c_cc[VKILL]
		% (int)term.c_cc[VEOF]
		% (int)term.c_cc[VMIN]
		% (int)term.c_cc[VEOL]
		% (int)term.c_cc[VTIME]
		% (int)term.c_cc[VSTOP]
		% (int)term.c_cc[VSUSP];
}
#endif

}} // pfs::io

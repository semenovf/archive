/**
 * @file   iodevice.hpp
 * @author wladt
 * @date   Apr 11, 2013 2:58:04 PM
 *
 * @brief
 */

#ifndef __CWT_IODEVICE_HPP__
#define __CWT_IODEVICE_HPP__

#include <cwt/cwt.h>
#include <cwt/bytearray.hpp>
#include <cwt/errorable.hpp>
#include <cwt/shared_ptr.hpp>

#define CWT_IODEVICE_PIMPL(Class)                          \
protected:                                                 \
	virtual ssize_t readBytes(char bytes[], size_t n) = 0; \
	virtual ssize_t writeBytes(const char bytes[], size_t n) = 0; \
	                                                       \
public:                                                    \
	virtual ~Class() { ; }                                 \
                                                           \
	Class& Class::operator=(const Class &other)            \
	{                                                      \
		pimpl = other.pimpl;                               \
		return *this;                                      \
	}                                                      \
	virtual size_t bytesAvailable();                       \
	virtual int close();                                   \
	virtual bool opened() const;                           \
    virtual IODevice* dup() { return new Class(*this); }   \
private:                                                   \
    class Impl;                                            \
    shared_ptr<Impl> pimpl;


CWT_NS_BEGIN

class IODevice : public Errorable
{
public:
	static const size_t ChunkSize = 512;

protected:
	IODevice();

public:
	enum OpenMode {
		  ReadOnly    = 0
		, WriteOnly   = 0x0001
		, ReadWrite   = 0x0002
		, WriteRead   = ReadWrite
		, NonBlocking = 0x0004
		, Unbuffered  = 0x0008
	};

protected:
	virtual ssize_t   readBytes(char bytes[], size_t n) = 0;
	virtual ssize_t   writeBytes(const char bytes[], size_t n) = 0;

public:
	virtual ssize_t   bytesAvailable() = 0;
	virtual int       close() = 0;
	virtual bool      opened() const = 0;
	virtual IODevice* dup() = 0;

	ssize_t read(char bytes[], size_t n)        { return readBytes(bytes, n); }
	ssize_t write(const char bytes[], size_t n) { return writeBytes(bytes, n); }
	virtual ByteArray readAll();
};

CWT_NS_END

#endif /* __CWT_IODEVICE_HPP__ */

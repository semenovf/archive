/**
 * @file   reader.hpp
 * @author wladt
 * @date   Feb 27, 2013 3:01:42 PM
 *
 * @brief
 */

#ifndef __CWT_READER_HPP__
#define __CWT_READER_HPP__

#include <cwt/cwt.h>
#include <cwt/string.hpp>
#include <QtCore/QIODevice>
#include <QtCore/QByteArray>
#include <QtCore/QFile>

CWT_NS_BEGIN

class AbstractReader {
protected:
	AbstractReader() {}
public:
	virtual ~AbstractReader() {}
	virtual bool	atEnd() const = 0;
	virtual long_t	bytesAvailable() const = 0;
	virtual long_t	bytesToWrite() const = 0;
	virtual bool	canReadLine() const = 0;
	virtual void	close() = 0;
	virtual bool    isSequential() const = 0;
	//bool	        isTextModeEnabled() const;
	//bool	        isWritable() const { return false; }
	virtual bool    open(int mode) = 0;
	//int openMode() const;
	//qint64	peek(char * data, qint64 maxSize);
	//QByteArray	peek(qint64 maxSize);
	virtual long_t	pos() const = 0;
	//bool	putChar(char c)
	//qint64	read(char * data, qint64 maxSize)
	//QByteArray	read(qint64 maxSize)
	//QByteArray	readAll()
	//qint64	readLine(char * data, qint64 maxSize)
	//QByteArray	readLine(qint64 maxSize = 0)
	virtual bool	reset() = 0;
	virtual bool	seek(long_t pos) = 0;
	//void	setTextModeEnabled(bool enabled);
	virtual long_t	size() const = 0;
	//void	ungetChar(char c)
	virtual bool	waitForReadyRead(int msecs) = 0;
	//qint64	write(const char * data, qint64 maxSize)
	//qint64	write(const char * data)
	//qint64	write(const QByteArray & byteArray)
};

class Reader : public QIODevice {
public:
	Reader() : QIODevice() {}
	virtual	~Reader() {}
	virtual bool	atEnd() const               = 0;
	virtual qint64	bytesAvailable() const      = 0;
	virtual bool	canReadLine() const         = 0;
	virtual void	close()                     = 0;
	virtual bool    isSequential() const        = 0; // regular files are not sequential
	virtual bool	open(OpenMode mode = QIODevice::ReadOnly) { CWT_UNUSED(mode); return QIODevice::open(ReadOnly); }
	virtual qint64	pos() const                 = 0;
	virtual bool	reset()                     = 0;
	virtual bool	seek(qint64 pos)            = 0;
	virtual qint64	size() const                = 0;
	virtual bool	waitForReadyRead(int msecs) { return QIODevice::waitForReadyRead(msecs); }
protected:
	virtual qint64  readData(char*, qint64)     = 0;

private:
	virtual qint64	bytesToWrite() const             { return qint64(-1); }
	virtual bool	waitForBytesWritten(int msecs)   { CWT_UNUSED(msecs); return false; }
	virtual qint64  writeData(const char *data, qint64 maxSize) { CWT_UNUSED2(data, maxSize); return qint64(-1); }
};

class FileReader : public Reader {
public:
	FileReader(const String &name);
	virtual ~FileReader() {}

	virtual bool	atEnd() const               { return impl.atEnd(); }
	virtual qint64	bytesAvailable() const      { return impl.bytesAvailable(); }
	virtual bool	canReadLine() const         { return impl.canReadLine(); }
	virtual void	close()                     { return impl.close(); }
	virtual bool    isSequential() const        { return impl.isSequential(); }
	virtual bool	open(OpenMode mode = QIODevice::ReadOnly) { CWT_UNUSED(mode); return impl.open(ReadOnly); }
	virtual qint64	pos() const                 { return impl.pos(); }
	virtual bool	reset()                     { return impl.reset(); }
	virtual bool	seek(qint64 pos)            { return impl.seek(pos); }
	virtual qint64	size() const                { return impl.size(); }
protected:
	virtual qint64  readData(char *data, qint64 maxSize)     { CWT_TRACE("readData"); return impl.read(data, maxSize); }
private:
	QFile impl;
};

CWT_NS_END

#endif /* __CWT_READER_HPP__ */

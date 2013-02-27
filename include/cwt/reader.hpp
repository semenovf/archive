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
	Reader() : QIODevice(), yes(true), no(false) {}
	virtual	~Reader() {}
	virtual bool	atEnd() const                    = 0; //{ CWT_ASSERT(yes == no); return true; }
	virtual qint64	bytesAvailable() const           = 0; //{ CWT_ASSERT(yes == no); return 1LL; }
	virtual qint64	bytesToWrite() const             = 0; //{ CWT_ASSERT(yes == no); return 1LL; }
	virtual bool	canReadLine() const              = 0; //{ CWT_ASSERT(yes == no); return false; }
	virtual void	close()                          = 0; //{ CWT_ASSERT(yes == no); }
	virtual bool    isSequential() const             = 0; //{ CWT_ASSERT(yes == no); return true; }
	virtual bool	open(OpenMode mode)              = 0; //{ CWT_ASSERT(yes == no); CWT_UNUSED(mode); return true; }
	virtual qint64	pos() const                      = 0; //{ CWT_ASSERT(yes == no); return 1LL; }
	virtual bool	reset()                          = 0; //{ CWT_ASSERT(yes == no); return true; }
	virtual bool	seek(qint64 pos)                 = 0; //{ CWT_ASSERT(yes == no); CWT_UNUSED(pos); return true; }
	virtual qint64	size() const                     = 0; //{ CWT_ASSERT(yes == no); return 1LL;}
	virtual bool	waitForReadyRead(int msecs)      = 0; //{ CWT_ASSERT(yes == no); CWT_UNUSED(msecs); return true; }
protected:
	virtual qint64  readData(char*, qint64)          = 0; //{ CWT_ASSERT(yes == no); CWT_UNUSED(msecs); return 1LL; }

public:
	bool	getChar(char * c)                        { CWT_ASSERT(yes == no); CWT_UNUSED(c); return false; }
	bool	isOpen() const                           { CWT_ASSERT(yes == no); return true; }
	bool	isReadable() const                       { CWT_ASSERT(yes == no); return true; }
	bool	isTextModeEnabled() const                { CWT_ASSERT(yes == no); return true; }
	bool	isWritable() const                       { CWT_ASSERT(yes == no); return false; }
	OpenMode openMode() const                        { CWT_ASSERT(yes == no); return QIODevice::ReadWrite; }
	qint64	peek(char * data, qint64 maxSize)        { CWT_ASSERT(yes == no); CWT_UNUSED2(data, maxSize); return 1LL; }
	QByteArray	peek(qint64 maxSize)                 { CWT_ASSERT(yes == no); CWT_UNUSED(maxSize);  return QByteArray(); }
	qint64	read(char * data, qint64 maxSize)        { CWT_ASSERT(yes == no); CWT_UNUSED2(data, maxSize); return 1LL; }
	QByteArray	read(qint64 maxSize)                 { CWT_ASSERT(yes == no); CWT_UNUSED(maxSize); return QByteArray(); }
	QByteArray	readAll()                            { CWT_ASSERT(yes == no); CWT_ASSERT(yes == no); return QByteArray(); }
	qint64	readLine(char * data, qint64 maxSize)    { CWT_ASSERT(yes == no); CWT_UNUSED2(data, maxSize); return 1LL;}
	QByteArray	readLine(qint64 maxSize = 0)         { CWT_ASSERT(yes == no); CWT_UNUSED(maxSize); return QByteArray(); }
	void	setTextModeEnabled(bool enabled)         { CWT_ASSERT(yes == no); CWT_UNUSED(enabled); }
	void	ungetChar(char c)                        { CWT_ASSERT(yes == no); CWT_UNUSED(c); }

	QString	errorString() const                      { CWT_ASSERT(yes == no); return QString(""); }
	bool	putChar(char c)                          { CWT_ASSERT(yes == no); CWT_UNUSED(c); return true; }
	virtual bool	waitForBytesWritten(int msecs)   { CWT_ASSERT(yes == no); CWT_UNUSED(msecs); return true; }
	qint64	write(const char * data, qint64 maxSize) { CWT_ASSERT(yes == no); CWT_UNUSED2(data, maxSize);  return 1LL; }
	qint64	write(const char * data)                 { CWT_ASSERT(yes == no); CWT_UNUSED(data); return 1LL; }
	qint64	write(const QByteArray & byteArray)      { CWT_ASSERT(yes == no); CWT_UNUSED(byteArray); return 1LL;  }
protected:
	virtual qint64 writeData(const char *data, qint64 maxSize) { CWT_ASSERT(yes == no); CWT_UNUSED2(data, maxSize); return 1LL; }

protected:
	bool yes;
	bool no;
};

class FileReader : public Reader {
public:
	FileReader() : Reader() {};
	virtual ~FileReader() {}
	virtual bool	atEnd() const                    { CWT_ASSERT(yes == no); return false; }
	virtual qint64	bytesAvailable() const           { CWT_ASSERT(yes == no); return 1LL; }
	virtual qint64	bytesToWrite() const             { CWT_ASSERT(yes == no); return 1LL; }
	virtual bool	canReadLine() const              { CWT_ASSERT(yes == no); return false; }
	virtual void	close()                          { CWT_ASSERT(yes == no); }
	virtual bool    isSequential() const             { CWT_ASSERT(yes == no); return true; }
	virtual bool	open(OpenMode mode)              { CWT_ASSERT(yes == no); CWT_UNUSED(mode); return true; }
	virtual qint64	pos() const                      { CWT_ASSERT(yes == no); return 1LL; }
	virtual bool	reset()                          { CWT_ASSERT(yes == no); return true; }
	virtual bool	seek(qint64 pos)                 { CWT_ASSERT(yes == no); CWT_UNUSED(pos); return true; }
	virtual qint64	size() const                     { CWT_ASSERT(yes == no); return 1LL;}
	virtual bool	waitForReadyRead(int msecs)      { CWT_ASSERT(yes == no); CWT_UNUSED(msecs); return true; }
protected:
	virtual qint64  readData(char *data, qint64 maxSize) { CWT_ASSERT(yes == no); CWT_UNUSED2(data, maxSize); return 1LL; }

private:
	QFile impl;
};


CWT_NS_END

#endif /* __CWT_READER_HPP__ */

/**
 * @file   logger.hpp
 * @author wladt
 * @date   Feb 13, 2013 11:16:20 AM
 *
 * @brief
 */

#ifndef __CWT_LOG_HPP__
#define __CWT_LOG_HPP__

#include <pfs/string.hpp>
#include <pfs/stringlist.hpp>
#include <cwt/safeformat.hpp>
#include <cwt/sigslot.hpp>
#include <cstdio>

namespace cwt {

class appender;

class log
{
public:
	enum priority { Trace, Debug, Info, Warn, Error, Fatal, NoLog };

private:
	signal1<const pfs::string &> _emitter;
	pfs::string _pattern;

public:
	void connect (appender &);
	void disconnect (appender &);
	void disconnectAll ();

	pfs::string pattern () const { return _pattern; }
	void setPattern (const pfs::string & pattern) { _pattern = pattern; }

	static void setPriority (priority level);
	static log::priority level ();

protected:
	static pfs::string patternify (log::priority level, const pfs::string & pattern, const pfs::string & text);
	pfs::string patternify (log::priority level, const pfs::string & text) { return patternify(level, _pattern, text); }
};

class appender : public has_slots<>
{
protected:
	pfs::string _pattern;

public:
	appender () : _pattern("%m") { }
	appender (const pfs::string &pattern) : /*_connected(false), */_pattern(pattern) {}
	virtual ~appender () {}

	pfs::string pattern () const { return _pattern; }
	void setPattern (const pfs::string & pattern) { _pattern = pattern; }

	virtual void operator () (const pfs::string &) = 0;
};

cwt::log & trace ();
cwt::log & debug ();
cwt::log & info  ();
cwt::log & warn  ();
cwt::log & error ();
cwt::log & fatal ();

void trace (const pfs::string & text);
void debug (const pfs::string & text);
void info  (const pfs::string & text);
void warn  (const pfs::string & text);
void warn  (int errn, const pfs::string & text);
void error (const pfs::string & text);
void error (int errn, const pfs::string & text);
void fatal (const pfs::string & text);
void fatal (int errn, const pfs::string & text);

inline void trace (const char * latin1) { trace(pfs::string(latin1)); }
inline void debug (const char * latin1) { debug(pfs::string(latin1)); }
inline void info  (const char * latin1) { info(pfs::string(latin1)); }
inline void warn  (const char * latin1) { warn(pfs::string(latin1)); }
inline void warn  (int errn, const char * latin1) { warn(errn, pfs::string(latin1)); }
inline void error (const char * latin1) { error(pfs::string(latin1)); }
inline void error (int errn, const char * latin1) { error(errn, pfs::string(latin1)); }
inline void fatal (const char * latin1) { fatal(pfs::string(latin1)); }
inline void fatal (int errn, const char * latin1) { fatal(errn, pfs::string(latin1)); }


/* Pattern flags:
 * 	m 	message associated with the logging event.
 * */
#ifdef __COMMENT__
class appender : public has_slots<>
{
public:
	void connect ();
	void disconnect ();
public:
	appender () : _connected(false), _pattern("%m"), _level(log::Trace) { }
	appender (const pfs::string &pattern) : _connected(false), _pattern(pattern), _level(log::Trace) {}
	virtual ~appender () { if(_connected) disconnect(); }

	void setPattern (const pfs::string & pattern) { _pattern = pattern; }
	void setPriority (priority level);
	log::priority level () { return _level; }

	virtual void trace (const pfs::string &) = 0;
	virtual void debug (const pfs::string &) = 0;
	virtual void info  (const pfs::string &) = 0;
	virtual void warn  (const pfs::string &) = 0;
	virtual void error (const pfs::string &) = 0;
	virtual void fatal (const pfs::string &) = 0;

protected:
	static pfs::string patternify (priority level, const pfs::string & pattern, const pfs::string & text);
	pfs::string patternify (priority level, const pfs::string & text) { return patternify(level, _pattern, text); }

protected:
	bool        _connected;
	pfs::string _pattern;
	priority    _level;
};


class null_appender : public appender
{
public:
	null_appender () {}
	~null_appender () {}
	virtual void trace (const pfs::string &) {}
	virtual void debug (const pfs::string &) {}
	virtual void info  (const pfs::string &) {}
	virtual void warn  (const pfs::string &) {}
	virtual void error (const pfs::string &) {}
	virtual void fatal (const pfs::string &) {}
};


class stdio_appender : public appender
{
public:
	stdio_appender () {}
	~stdio_appender () {}
	virtual void trace (const pfs::string & text) { ::fprintf(stdout, "%s\n", patternify(log::Trace, text).c_str()); }
	virtual void debug (const pfs::string & text) { ::fprintf(stdout, "%s\n", patternify(log::Debug, text).c_str()); }
	virtual void info  (const pfs::string & text) { ::fprintf(stdout, "%s\n", patternify(log::Info, text).c_str()); }
	virtual void warn  (const pfs::string & text) { ::fprintf(stderr, "%s\n", patternify(log::Warn, text).c_str()); }
	virtual void error (const pfs::string & text) { ::fprintf(stderr, "%s\n", patternify(log::Error, text).c_str()); }
	virtual void fatal (const pfs::string & text) { ::fprintf(stderr, "%s\n", patternify(log::Fatal, text).c_str()); }
};

class strings_appender : public appender
{
public:
	strings_appender() {}
	~strings_appender() {}
	virtual void trace (const pfs::string & text) { _traceStrings.append(patternify(log::Trace, text)); }
	virtual void debug (const pfs::string & text) { _debugStrings.append(patternify(log::Debug, text)); }
	virtual void info  (const pfs::string & text) { _infoStrings.append(patternify (log::Info, text)); }
	virtual void warn  (const pfs::string & text) { _warnStrings.append(patternify (log::Warn, text)); }
	virtual void error (const pfs::string & text) { _errorStrings.append(patternify(log::Error, text)); }
	virtual void fatal (const pfs::string & text) { _fatalStrings.append(patternify(log::Fatal, text)); }

	pfs::stringlist & traceStrings () { return _traceStrings; }
	pfs::stringlist & debugStrings () { return _debugStrings; }
	pfs::stringlist & infoStrings  () { return _infoStrings; }
	pfs::stringlist & warnStrings  () { return _warnStrings; }
	pfs::stringlist & errorStrings () { return _errorStrings; }
	pfs::stringlist & fatalStrings () { return _fatalStrings; }
	const pfs::stringlist & traceStrings () const { return _traceStrings; }
	const pfs::stringlist & debugStrings () const { return _debugStrings; }
	const pfs::stringlist & infoStrings  () const { return _infoStrings; }
	const pfs::stringlist & warnStrings  () const { return _warnStrings; }
	const pfs::stringlist & errorStrings () const { return _errorStrings; }
	const pfs::stringlist & fatalStrings () const { return _fatalStrings; }

	void clear ();

private:
	pfs::stringlist _traceStrings;
	pfs::stringlist _debugStrings;
	pfs::stringlist _infoStrings;
	pfs::stringlist _warnStrings;
	pfs::stringlist _errorStrings;
	pfs::stringlist _fatalStrings;
};

inline void connectAppender (appender * appender) { appender->connect(); }
inline void disconnectAppender (appender * appender) { appender->disconnect(); }
void restoreDefaultAppenders ();
void disconnectAllAppenders ();

#endif

} // cwt

#endif /* __CWT_LOG_HPP__ */

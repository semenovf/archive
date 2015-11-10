/**
 * @file   logger.hpp
 * @author wladt
 * @date   Feb 13, 2013 11:16:20 AM
 *
 * @brief
 */

#ifndef __PFS_LOGGER_HPP__
#define __PFS_LOGGER_HPP__

#include <pfs/string.hpp>
#include <pfs/stringlist.hpp>
#include <pfs/safeformat.hpp>
#include <pfs/sigslot.hpp>
#include <pfs/notification.hpp>
#include <iostream>


#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

class appender;

class DLL_API log
{
public:
	enum priority { Trace, Debug, Info, Warn, Error, Fatal, NoLog };
	static pfs::string DefaultPattern;
	static pfs::string NoPattern;

private:
	pfs::signal2<priority, const pfs::string &> _emitter;

public:
	log (appender & a) { connect(a); }
	void print (log::priority level, const pfs::string & msg) { _emitter(level, msg); }
	void connect (appender &);
	void disconnect (appender &);
	void disconnectAll ();

	static void setPriority (priority level);
	static log::priority level ();
	static void disconnectAllAppenders ();
	static void restoreDefaultAppenders ();

	static void print (const notification & nx);
};

class appender : public has_slots<>
{
	friend class log;

protected:
	pfs::string _pattern;

protected:
	virtual void print (const pfs::string & msg) = 0;

	void print_helper (log::priority level, const pfs::string & msg);
	static pfs::string patternify (log::priority level, const pfs::string & pattern, const pfs::string & text);

public:
	appender () : _pattern(log::DefaultPattern) { }
	appender (const pfs::string & pattern) : _pattern(pattern) {}
	virtual ~appender () {}

	pfs::string pattern () const { return _pattern; }
	void setPattern (const pfs::string & pattern) { _pattern = pattern; }
};

class stdout_appender : public appender
{
public:
	stdout_appender () : appender() {}
protected:
	virtual void print (const pfs::string & msg) override { std::cout << msg << std::endl; }
};

class stderr_appender : public appender
{
public:
	stderr_appender () : appender() {}
protected:
	virtual void print (const pfs::string & msg) override { std::cerr << msg << std::endl; }
};

class stringlist_appender : public appender
{
	pfs::stringlist _strings;
public:
	stringlist_appender () {}
	const pfs::stringlist & data () const { return _strings; }
	pfs::stringlist & data () { return _strings; }

protected:
	virtual void print (const pfs::string & msg) override { _strings.append(msg); }
};

inline void log::connect (appender & a)
{
	_emitter.connect(& a, & appender::print_helper);
}

inline void log::disconnect (appender & a)
{
	_emitter.disconnect(& a);
}

inline void log::disconnectAll ()
{
	_emitter.disconnect_all();
}

DLL_API log & trace ();
DLL_API log & debug ();
DLL_API log & info  ();
DLL_API log & warn  ();
DLL_API log & error ();
DLL_API log & fatal ();

DLL_API void trace (const pfs::string & text);
DLL_API void debug (const pfs::string & text);
DLL_API void info  (const pfs::string & text);
DLL_API void warn  (const pfs::string & text);
DLL_API void warn  (int errn, const pfs::string & text);
DLL_API void error (const pfs::string & text);
DLL_API void error (int errn, const pfs::string & text);
DLL_API void fatal (const pfs::string & text);
DLL_API void fatal (int errn, const pfs::string & text);

inline void trace (const char * latin1) { trace(pfs::string(latin1)); }
inline void debug (const char * latin1) { debug(pfs::string(latin1)); }
inline void info  (const char * latin1) { info(pfs::string(latin1)); }
inline void warn  (const char * latin1) { warn(pfs::string(latin1)); }
inline void warn  (int errn, const char * latin1) { warn(errn, pfs::string(latin1)); }
inline void error (const char * latin1) { error(pfs::string(latin1)); }
inline void error (int errn, const char * latin1) { error(errn, pfs::string(latin1)); }
inline void fatal (const char * latin1) { fatal(pfs::string(latin1)); }
inline void fatal (int errn, const char * latin1) { fatal(errn, pfs::string(latin1)); }

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_LOGGER_HPP__ */

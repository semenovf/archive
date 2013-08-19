/*
 * dbd.hpp
 *
 *  Created on: Aug 19, 2013
 *      Author: wladt
 */

#ifndef __CWT_DBD_HPP__
#define __CWT_DBD_HPP__

#include <cwt/vector.hpp>
#include <cwt/errorable.hpp>

CWT_NS_BEGIN

class DbHandlerData {};
class DbStatementData {};

class DbDriver
{
	static DbHandlerData *         (*connect)       (const String & uri
			, const String & username
			, const String & password
			, const String & csname);
	static void                    (*disconnect)    (DbHandlerData &);
	static bool                    (*func)          (DbHandlerData & , const String &, String *);
	static void                    (*attr)          (DbHandlerData & , const String &, void *);
	static bool                    (*setAutoCommit) (DbHandlerData & , bool);
	static bool                    (*autoCommit)    (DbHandlerData & );
	static long_t                  (*errno)         (DbHandlerData &);
	static String                  (*strerror)      (DbHandlerData &);
	static String                  (*state)         (DbHandlerData &);
	static bool                    (*query)         (DbHandlerData &, const String & sql);   /* cannot be used for statements that contain binary data */
	static DbStatementData *       (*prepare)       (DbHandlerData &, const String & sql);
	static ulong_t                 (*rows)          (DbHandlerData &);
	static Vector<String>          (*tables)        (DbHandlerData &);
	static bool                    (*tableExists)   (DbHandlerData &, const String & tname);

	static char *                  (*encode_n)      (DbHandlerData &, const String & s, size_t n);
	static char *                  (*encode)        (DbHandlerData &, const String & s);
	static String                  (*decode)        (DbHandlerData &, const char * s);
	static String                  (*decode_n)      (DbHandlerData &, const char * s, size_t n);

	static bool                    (*begin)         (DbHandlerData &); /* begin transaction */
	static bool                    (*commit)        (DbHandlerData &);
	static bool                    (*rollback)      (DbHandlerData &);
};

CWT_NS_END

#endif /* __CWT_DBD_HPP__ */

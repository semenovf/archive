/*
 * dbi.h
 *
 *  Created on: 03.07.2012
 *      Author: wladt
 *
 * Database interface based on Perl's DBI API subset
 *
 */

#ifndef __CWT_DBI_H__
#define __CWT_DBI_H__

#include <cwt/types.h>

typedef INT32 CwtDBI_RC;

typedef struct CwtSqlDate {
	UINT  year
		, month
		, day;
} CwtSqlDate;

typedef struct CwtSqlTime {
	UINT  hour
		, minute
		, second;
	ULONG second_part;
} CwtSqlTime;

typedef struct CwtSqlTimeStamp {
	UINT  year
		, month
		, day;
	UINT  hour
		, minute
		, second;
	ULONG second_part;
} CwtSqlTimeStamp;


typedef struct CwtStatementBase {
	void        (*close)(struct CwtStatementBase*);
	BOOL        (*execute)(struct CwtStatementBase*);
	CwtDBI_RC   (*err)(struct CwtStatementBase*);
	const CHAR* (*errstr)(struct CwtStatementBase*);
	BOOL        (*bind)(struct CwtStatementBase*, size_t index, CwtTypeId type_id, void *value, size_t *plength);
	ULONGLONG   (*nrows)(struct CwtStatementBase*);
} *CwtStatement;

typedef struct CwtDBHandlerBase
{
	void         (*disconnect)(struct CwtDBHandlerBase *dbh);
	BOOL         (*func)(struct CwtDBHandlerBase*, const CHAR*, CHAR**);
	void         (*attr)(struct CwtDBHandlerBase*, const CHAR*, void*);
	BOOL         (*setAutoCommit)(struct CwtDBHandlerBase*, BOOL);
	BOOL         (*autoCommit)(struct CwtDBHandlerBase*);
	CwtDBI_RC    (*err)(struct CwtDBHandlerBase*);
	const CHAR*  (*errstr)(struct CwtDBHandlerBase*);
	const CHAR*  (*state)(struct CwtDBHandlerBase*);
	BOOL         (*query)(struct CwtDBHandlerBase*, const CHAR *sql);   /* cannot be used for statements that contain binary data */
	BOOL         (*queryBin)(struct CwtDBHandlerBase*, const CHAR *sql, size_t length); /* can be used for statements that contain binary data */
	CwtStatement (*prepare)(struct CwtDBHandlerBase*, const CHAR *sql);
	ULONGLONG    (*nrows)(struct CwtDBHandlerBase*);

	/*INT32       (*doStatement)(struct CwtDBHandlerBase*);*/
} *CwtDBHandler;




EXTERN_C_BEGIN

DLL_API_EXPORT void         cwtDBIParseDSN(const CHAR *dsn, CHAR **scheme, CHAR **driver, CHAR **driverDSN);
DLL_API_EXPORT CwtDBHandler cwtDBIConnect(const CHAR *dsn, const CHAR *username, const CHAR *password);
#ifdef __NOT_IMPLEMENTED_YET__
DLL_API_EXPORT CwtDBHandler cwtDBIConnectCached(const CHAR *dsn, const CHAR *username, const CHAR *password);
#endif

EXTERN_C_END

#endif /* __CWT_DBI_H__ */

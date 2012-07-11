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


typedef struct CwtBindEntry {
	CwtTypeId type_id;
	void *value;
	size_t *plength;
} CwtBindEntry;


typedef struct CwtStatement {
	CHAR unused;
} CwtStatement;

typedef struct CwtDBHandler {
	void            (*close)         (CwtStatement*);
	BOOL            (*execute)       (CwtStatement*);
	CwtDBI_RC       (*err)           (CwtStatement*);
	const CWT_CHAR* (*strerror)      (CwtStatement*);
	BOOL            (*bind)          (CwtStatement*, size_t index, CwtTypeId type_id, void *value, size_t *plength);
	BOOL            (*bindArray)     (CwtStatement*, CwtBindEntry bindArray[]);
	ULONGLONG       (*rows)          (CwtStatement*);
} CwtDBHandler;

typedef struct CwtDBIDriver
{
	CwtDBHandler*   (*connect)       (const CWT_CHAR *dsn, const CWT_CHAR *username, const CWT_CHAR *password, const CWT_CHAR *csname);
	void            (*disconnect)    (CwtDBHandler*);
	BOOL            (*func)          (CwtDBHandler*, const CWT_CHAR*, CWT_CHAR**);
	void            (*attr)          (CwtDBHandler*, const CWT_CHAR*, void*);
	BOOL            (*setAutoCommit) (CwtDBHandler*, BOOL);
	BOOL            (*autoCommit)    (CwtDBHandler*);
	CwtDBI_RC       (*err)           (CwtDBHandler*);
	const CWT_CHAR* (*strerror)      (CwtDBHandler*);
	const CWT_CHAR* (*state)         (CwtDBHandler*);
	BOOL            (*query)         (CwtDBHandler*, const CWT_CHAR *sql);   /* cannot be used for statements that contain binary data */
	BOOL            (*queryBin)      (CwtDBHandler*, const CWT_CHAR *sql, size_t length); /* can be used for statements that contain binary data */
	CwtStatement*   (*prepare)       (CwtDBHandler*, const CWT_CHAR *sql);
	ULONGLONG       (*rows)          (CwtDBHandler*);
	char*           (*encode)        (CwtDBHandler*, const CWT_CHAR *s);
	CWT_CHAR*       (*decode)        (CwtDBHandler*, const char *s);
} CwtDBIDriver;

typedef struct CwtDBI
{
	void            (*parseDSN)      (const CWT_CHAR *dsn, CWT_CHAR **scheme, CWT_CHAR **driver, CWT_CHAR **driverDSN);
	CwtDBIDriver*   (*load)          (const CWT_CHAR *dsn);
} CwtDBI;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtDBI* cwtDBI(void);
EXTERN_C_END

#endif /* __CWT_DBI_H__ */

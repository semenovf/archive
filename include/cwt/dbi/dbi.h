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
#include <cwt/strlist.h>

typedef INT32 CwtDBI_RC;

/*
typedef struct CwtBindEntry {
	CwtTypeId type_id;
	void     *value;
	size_t   *plength;
} CwtBindEntry;
*/

typedef enum CwtTypeId {
	  CwtType_NULL
	, CwtType_CHAR
	, CwtType_UCHAR
	, CwtType_SHORT
	, CwtType_USHORT
	, CwtType_INT
	, CwtType_UINT
	, CwtType_LONG
	, CwtType_ULONG
	, CwtType_LONGLONG
	, CwtType_ULONGLONG
	, CwtType_FLOAT
	, CwtType_DOUBLE
	, CwtType_STRING
	, CwtType_TEXT
	, CwtType_BLOB
	, CwtType_TIME
	, CwtType_DATE
	, CwtType_DATETIME
	, CwtType_SBYTE  = CwtType_CHAR
	, CwtType_BYTE   = CwtType_UCHAR
	, CwtType_INT8   = CwtType_SBYTE
	, CwtType_UINT8  = CwtType_BYTE
	, CwtType_INT16  = CwtType_SHORT
	, CwtType_UINT16 = CwtType_USHORT
	, CwtType_INT32  = CwtType_LONG
	, CwtType_UINT32 = CwtType_ULONG
	, CwtType_INT64  = CwtType_LONGLONG
	, CwtType_UINT64 = CwtType_ULONGLONG
} CwtTypeId;

#define CWT_TYPEID_IS_NUMBER(tid) (((tid) > CwtType_NULL && (tid) < CwtType_STRING) ? TRUE : FALSE)


typedef struct CwtStatement {
	CHAR unused;
} CwtStatement;

typedef struct CwtDBHandler {
	void            (*close)         (CwtStatement*);
	BOOL            (*execute)       (CwtStatement*);
	CwtDBI_RC       (*err)           (CwtStatement*);
	const CWT_CHAR* (*strerror)      (CwtStatement*);
	BOOL            (*bind)          (CwtStatement *sth, size_t index, CwtTypeId type_id, void *value, size_t *plength);
	BOOL            (*bindScalar)    (CwtStatement *sth, size_t index, CwtTypeId type_id, void *value);
	BOOL            (*bindTime)      (CwtStatement *sth, size_t index, CwtTypeId type_id, void *value);
	BOOL            (*bindNull)      (CwtStatement *sth, size_t index);
	ULONGLONG       (*rows)          (CwtStatement*);
	ULONGLONG       (*size)          (CwtStatement*);
	BOOL            (*fetchNext)     (CwtStatement*);
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
	BOOL            (*tables)        (CwtDBHandler*, CwtStrList *tables);
	char*           (*encode)        (CwtDBHandler*, const CWT_CHAR *s);
	CWT_CHAR*       (*decode)        (CwtDBHandler*, const char *s);
	CWT_TIME*       (*createTime)    (void);
	void            (*freeTime)      (CWT_TIME*);
	BOOL            (*begin)         (CwtDBHandler*); /* begin transaction */
	BOOL            (*commit)        (CwtDBHandler*);
	BOOL            (*rollback)      (CwtDBHandler*);
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

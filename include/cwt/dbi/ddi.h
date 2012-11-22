/*
 * ddi.h
 *
 *  Created on: 30.07.2012
 *      Author: wladt
 */

#ifndef __CWT_DDI_H__
#define __CWT_DDI_H__

#include <cwt/types.h>
#include <cwt/list.h>


typedef enum _CwtDdiFlag {
	  CwtDdi_NoFlags       = 0x0000 /*all: no flags, this is default value*/
	, CwtDdi_DeployDropDb  = 0x0001 /*deploy: drop database before creation*/
	, CwtDdi_DeployDropTab = 0x0002 /*deploy: drop table before creation*/
	, CwtDdi_RecallDropDb  = 0x0004 /*recall: drop database*/
} CwtDdiFlag;

typedef enum _CwtDdiColumnFlag {
	  CwtDdiColumn_General    = 0
	, CwtDdiColumn_Unique     = 0x0001
	, CwtDdiColumn_PK         = 0x0002 | CwtDdiColumn_Unique /* + Unique */
	, CwtDdiColumn_Indexable  = 0x0004
	, CwtDdiColumn_Nullable   = 0x0008
	, CwtDdiColumn_Insertable = 0x0010 /* TODO not used yet */
	, CwtDdiColumn_Updatable  = 0x0020 /* TODO not used yet */
} CwtDdiColumnFlag;

struct _CwtDDITable;

struct _CwtDDIColumnIntOpts   { ULONGLONG max;    LONGLONG min; };
struct _CwtDDIColumnFloatOpts { ULONGLONG prec;   LONGLONG scale; } ;
struct _CwtDDIColumnTextOpts  { ULONGLONG maxlen; LONGLONG unused; } ;
struct _CwtDDIColumnBlobOpts  { ULONGLONG maxlen; LONGLONG unused; } ;
struct _CwtDDIColumnTimeOpts  { ULONGLONG stamp;  LONGLONG unused; } ;
typedef union _CwtDDIColumnOpts {
	struct _CwtDDIColumnIntOpts   int_opts;
	struct _CwtDDIColumnFloatOpts float_opts;
	struct _CwtDDIColumnTextOpts  text_opts;
	struct _CwtDDIColumnBlobOpts  blob_opts;
	struct _CwtDDIColumnTimeOpts  time_opts;
} CwtDDIColumnOpts;

#define CWT_DDI_INT(_min,_max,_autoinc)        NULL , CwtType_INT      , {{(ULONGLONG)_max     , (LONGLONG)_min}}   , _autoinc  , NULL
#define CWT_DDI_FLOAT(_prec,_scale,_autoinc)   NULL , CwtType_FLOAT    , {{(ULONGLONG)_prec    , (LONGLONG)_scale}} , _autoinc  , NULL
#define CWT_DDI_DOUBLE(_prec,_scale,_autoinc)  NULL , CwtType_DOUBLE   , {{(ULONGLONG)_prec    , (LONGLONG)_scale}} , _autoinc  , NULL
#define CWT_DDI_TEXT(_maxlen)                  NULL , CwtType_TEXT     , {{(ULONGLONG)_maxlen  , 0LL}}              , 0         , NULL
#define CWT_DDI_BLOB(_maxlen)                  NULL , CwtType_BLOB     , {{(ULONGLONG)_maxlen  , 0LL}}              , 0         , NULL
#define CWT_DDI_TIME(is_stamp)                 NULL , CwtType_TIME     , {{(ULONGLONG)is_stamp , 0LL}}              , 0         , NULL
#define CWT_DDI_DATE(is_stamp)                 NULL , CwtType_DATE     , {{(ULONGLONG)is_stamp , 0LL}}              , 0         , NULL
#define CWT_DDI_DATETIME(is_stamp)             NULL , CwtType_DATETIME , {{(ULONGLONG)is_stamp , 0LL}}              , 0         , NULL
#define CWT_DDI_REF(_entity)                   NULL , CwtType_UNKNOWN  , {{0ULL                , 0LL}}              , 0         , (CwtDDITable*)_entity
#define CWT_DDI_BOOL                           NULL , CwtType_BOOL     , {{0ULL                , 0LL}}              , 0         , NULL
#define CWT_DDI_BOOL_TRUE                      _T("1")
#define CWT_DDI_BOOL_FALSE                     _T("0")
#define CWT_DDI_NULL_ENTRY                     { NULL, CWT_DDI_BOOL, CwtDdiColumn_General, NULL, NULL }

typedef struct _CwtDDIColumn {
	CWT_CHAR            *name;
	struct _CwtDDITable *owner_ptr;
	CwtTypeEnum          type;
	CwtDDIColumnOpts     opts;
	UINT                 autoinc; /* AUTO_INCREMENT value, valid for integer or floating point number, 0 - no auto increment */
	struct _CwtDDITable *ref_ptr;
	UINT32               flags;   /* see CwtDdiColumnFlag */
	CWT_CHAR            *default_value;
} CwtDDIColumn, CwtPersistEntry;

typedef struct _CwtDDITable {
	CWT_CHAR     *name;
	CwtDDIColumn *pk_ptr;      /* pointer to the primary key column */
	CwtDDIColumn *autoinc_ptr; /* pointer to the autoincremented column */
	CwtList      *columns;
} CwtDDITable;

typedef struct _CwtPersistEntity {
	CWT_CHAR        *name;
	CwtPersistEntry *entries;
	CwtDDITable     *table_ptr;
} CwtPersistEntity;


typedef struct _CwtDDI {
	CWT_CHAR     *name;
	CWT_CHAR     *charset;
	CwtList      *tables;
} CwtDDI;


#endif /* __CWT_DDI_H__ */

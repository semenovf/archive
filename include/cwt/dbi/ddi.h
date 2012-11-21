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
	, CwtDdiColumn_FK         = 0x0004 /* used only in persistence specification */
	                                   /* to reserve column as reference (Foreign Key) */
	, CwtDdiColumn_Indexable  = 0x0008
	, CwtDdiColumn_Nullable   = 0x0010
	, CwtDdiColumn_Insertable = 0x0020 /* TODO not used yet */
	, CwtDdiColumn_Updatable  = 0x0040 /* TODO not used yet */
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

#define CWT_DDI_INT_OPTS(_min,_max)       {{(ULONGLONG)_max     , (LONGLONG)_min}}
#define CWT_DDI_FLOAT_OPTS(_prec,_scale)  {{(ULONGLONG)_prec    , (LONGLONG)_scale}}
#define CWT_DDI_FLOAT_OPTS_DEFAULT        {{0ULL                , 0LL}}
#define CWT_DDI_TEXT_OPTS(_maxlen)        {{(ULONGLONG)_maxlen  , 0LL}}
#define CWT_DDI_BLOB_OPTS(_maxlen)        {{(ULONGLONG)_maxlen  , 0LL}}
#define CWT_DDI_TIME_OPTS(is_stamp)       {{(ULONGLONG)is_stamp , 0LL}}
#define CWT_DDI_BOOL_OPTS                 {{0ULL                , 0LL}}
#define CWT_DDI_FKL_OPTS                  {{0ULL                , 0LL}}

typedef struct _CwtDDIColumn {
	struct _CwtDDITable *pOwner;
	CWT_CHAR            *name;
	CwtTypeEnum          type;
	struct _CwtDDITable *pRef;
	CwtDDIColumnOpts     opts;

	CWT_CHAR *default_value;
	UINT autoinc; /* AUTO_INCREMENT value, valid for integer or floating point number, 0 - no auto increment */
	UINT32 flags; /* see CwtDdiColumnFlag */
} CwtDDIColumn;

typedef struct _CwtDDITable {
	CWT_CHAR     *name;
	CwtDDIColumn *pPK; /* Primary key */
	CwtDDIColumn *pAutoinc;
	CwtList      *columns;
} CwtDDITable;


typedef struct _CwtDDI {
	CWT_CHAR     *name;
	CWT_CHAR     *charset;
	CwtList      *tables;
} CwtDDI;


#endif /* __CWT_DDI_H__ */

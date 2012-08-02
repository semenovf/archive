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


typedef enum _CwtDDI_Flag {
	  CWT_DDI_NO_FLAGS                  = 0x0000 /*all: no flags, this is default value*/
	, CWT_DDI_CREATE_TAB_IF_NOT_EXISTS  = 0x0001 /*deploy: create table if it does not exist*/
	, CWT_DDI_DO_NOT_CREATE_DB          = 0x0002 /*deploy: do not create database if it does not exist*/
	, CWT_DDI_DROP_DB                   = 0x0004 /*recall: drop database*/
} CwtDDI_Flag;


struct _CwtDDITable;

typedef struct _CwtDDIColumn {
	struct _CwtDDITable *pOwner;
	CWT_CHAR            *name;
	CwtTypeEnum          type;
	struct _CwtDDITable *pRef;

	union {
		struct { LONGLONG min; ULONGLONG max; } int_opts;
		struct { UINT prec; UINT scale; } float_opts;
		ULONGLONG maxlen; /* for TEXTs or BLOBs*/
		BOOL stamp;       /* for time */
	} opts;

	UINT autoinc; /* AUTO_INCREMENT value, valid for integer or floating point number, 0 - no auto increment */
	int  is_null : 1;
	/*int                  is_uniq    : 1;*/
} CwtDDIColumn;

typedef struct _CwtDDITable {
	CWT_CHAR     *name;
	CwtDDIColumn *pPK; /* Primary key */
	CwtDDIColumn *pAutoinc;
	CwtList      *columns;
} CwtDDITable;


typedef struct _CwtDDI {
	CWT_CHAR     *name;
	CwtList      *tables;
} CwtDDI;


#endif /* __CWT_DDI_H__ */

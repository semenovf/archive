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


typedef enum _CwtDDI_Type {
	  CWT_DDI_TYPE_UNKNOWN
	, CWT_DDI_TYPE_BOOL = 1
	, CWT_DDI_TYPE_INT
    , CWT_DDI_TYPE_FLOAT
    , CWT_DDI_TYPE_TIME
    , CWT_DDI_TYPE_DATE
    , CWT_DDI_TYPE_DATETIME
    , CWT_DDI_TYPE_TIMESTAMP
    , CWT_DDI_TYPE_TEXT
    , CWT_DDI_TYPE_REF   /* reference (foreign key) to another table */
} CwtDDI_Type;

typedef enum _CwtDDI_Flag {
	  CWT_DDI_NO_FLAGS                  = 0x0000 /*all: no flags, this is default value*/
	, CWT_DDI_CREATE_TAB_IF_NOT_EXISTS  = 0x0001 /*deploy: create table if it does not exist*/
	, CWT_DDI_DO_NOT_CREATE_DB          = 0x0002 /*deploy: do not create database if it does not exist*/
	, CWT_DDI_DROP_DB                   = 0x0004 /*recall: drop database*/
} CwtDDI_Flag;


struct _CwtDDITable;

typedef struct _CwtDDIColumn {
	struct _CwtDDITable *table;
	CWT_CHAR            *name;
	CwtDDI_Type          type;
	struct _CwtDDITable *ref;
	int                  autoinc; /* AUTO_INCREMENT value, 0 - no auto increment */
	int                  is_null    : 1;
	/*int                  is_uniq    : 1;*/
} CwtDDIColumn;

typedef struct _CwtDDITable {
	CWT_CHAR     *name;
	CwtDDIColumn *pk; /* Primary key */
	CwtDDIColumn *autoinc;
	CwtList      *columns;
} CwtDDITable;


typedef struct _CwtDDI {
	CwtList      *tables;
} CwtDDI;


#endif /* __CWT_DDI_H__ */

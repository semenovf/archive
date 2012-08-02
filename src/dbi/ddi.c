/*
 * ddi.c
 *
 *  Created on: 30.07.2012
 *      Author: wladt
 */


#include <cwt/dbi/ddi.h>
#include <cwt/dbi/dbi.h>
#include <cwt/str.h>
#include <cwt/logger.h>


static CwtStrNS  *__strNS = NULL;
static CwtListNS *__listNS = NULL;


static inline void __clean_type(CwtDDIColumn *col)
{
	CWT_ASSERT(col);

	/* clear autoinc */
	if( !(CWT_TYPE_IS_INTEGER(col->type) || CWT_TYPE_IS_FLOAT(col->type)) ) {
		col->autoinc = 0;
		if( col->pOwner->pAutoinc == col )
			col->pOwner->pAutoinc = NULL;
	}

	/* clear opts */
	__strNS->bzero(&col->opts, sizeof(col->opts));

	/* clear reference */
	col->pRef = NULL;

	/* clear type */
	col->type = CwtType_UNKNOWN;

}


static void __ddi_cleanupTable(void *tab)
{
	CwtDDITable *t = (CwtDDITable*)tab;

	CWT_FREE(t->name);
	__listNS->free(t->columns);
	CWT_FREE(t);
}


static void __ddi_cleanupColumn(void *col)
{
	CwtDDIColumn *c = (CwtDDIColumn*)col;
	CWT_FREE(c->name);
	CWT_FREE(c);
}



CwtDDI* __ddi_createDDI(const CWT_CHAR *name)
{
	CwtDDI *ddi = CWT_MALLOC(CwtDDI);

	if( !__strNS ) {
		__strNS  = cwtStrNS();
		__listNS = cwtListNS();
	}

	__strNS->bzero(ddi, sizeof(CwtDDI));
	ddi->name = __strNS->strdup(name);
	ddi->tables = __listNS->create(sizeof(CwtDDITable), __ddi_cleanupTable);
	return ddi;
}

void __ddi_freeDDI(CwtDDI *ddi)
{
	CWT_ASSERT(ddi);

	if( ddi->name )
		CWT_FREE(ddi->name);

	if( ddi->tables )
		__listNS->free(ddi->tables);

	CWT_FREE(ddi);
}

CwtDDITable* __ddi_newTable(CwtDDI *ddi, const CWT_CHAR *name)
{
	CwtDDITable *tab;

	CWT_ASSERT(ddi);
	CWT_ASSERT(ddi->tables);

	tab = CWT_MALLOC(CwtDDITable);
	__strNS->bzero(tab, sizeof(CwtDDITable));
	tab->name = __strNS->strdup(name);
	tab->columns = __listNS->create(sizeof(CwtDDIColumn), __ddi_cleanupColumn);

	__listNS->append(ddi->tables, tab);

	return tab;
}

CwtDDIColumn* __ddi_newColumn(CwtDDITable *tab, const CWT_CHAR *name)
{
	CwtDDIColumn *col;

	CWT_ASSERT(tab);
	CWT_ASSERT(tab->columns);

	col = CWT_MALLOC(CwtDDIColumn);
	__strNS->bzero(col, sizeof(CwtDDIColumn));
	col->pOwner = tab;
	col->name  = __strNS->strdup(name);

	__listNS->append(tab->columns, col);
	return col;
}


BOOL __ddi_cTypeBool(CwtDDIColumn *col)
{
	__clean_type(col);
	col->type = CwtType_BOOL;
	return TRUE;
}

BOOL __ddi_cTypeInt(CwtDDIColumn *col, LONGLONG min, ULONGLONG max)
{
	__clean_type(col);
	col->type = CwtType_INT;
	col->opts.int_opts.min = min;
	col->opts.int_opts.max = max;
	return TRUE;
}

BOOL __ddi_cTypeFloat(CwtDDIColumn *col, UINT prec, UINT scale)
{
	__clean_type(col);
	col->type = CwtType_FLOAT;
	col->opts.float_opts.prec = prec;
	col->opts.float_opts.scale = scale;
	return TRUE;
}

BOOL __ddi_cTypeText(CwtDDIColumn *col, ULONGLONG maxlen)
{
	__clean_type(col);
	col->type = CwtType_TEXT;
	col->opts.maxlen = maxlen;
	return TRUE;
}

BOOL __ddi_cTypeBlob(CwtDDIColumn *col, ULONGLONG maxlen)
{
	__clean_type(col);
	col->type = CwtType_BLOB;
	col->opts.maxlen = maxlen;
	return TRUE;
}


/**
 *
 *
 * @param col
 * @param time_type
 * @param stamp if @c TRUE it is transformed into TIMESTAMP
 * @return
 */
BOOL __ddi_cTypeTime(CwtDDIColumn *col, CwtTypeEnum time_type, BOOL stamp)
{
	__clean_type(col);

	switch(time_type) {
	case CwtType_TIME:
	case CwtType_DATE:
	case CwtType_DATETIME:
		col->type = time_type;
		col->opts.stamp = stamp;
		return TRUE;
	default:
		break;
	}
	return FALSE;
}

BOOL __ddi_cTypeRef(CwtDDIColumn *col, CwtDDITable *ref)
{
	__clean_type(col);

	CWT_ASSERT(ref);
	CWT_ASSERT(ref->name);

	if( !ref->pPK ) {
		printf_error(_T("primary key not set for referenced table '%s'"), ref->name);
		return FALSE;
	}

	col->type = ref->pPK->type;
	__strNS->memcpy(&col->opts, &ref->pPK->opts, sizeof(ref->pPK->opts));
	col->pRef = ref;

	return TRUE;
}

BOOL __ddi_cAutoinc (CwtDDIColumn *col, UINT inc)
{
	CWT_ASSERT(col);
	CWT_ASSERT(col->name);

	if( CWT_TYPE_IS_INTEGER(col->type) || CWT_TYPE_IS_FLOAT(col->type)) {
		col->autoinc = inc;
		col->pOwner->pAutoinc = col;
	}

	return TRUE;
}

BOOL __ddi_cNull(CwtDDIColumn *col, BOOL yes)
{
	CWT_ASSERT(col);
	col->is_null = yes ? 1 : 0;
	return TRUE;
}


BOOL __ddi_deploy(CwtDBHandler *dbh, CwtDDI *ddi)
{
	CWT_UNUSED(dbh);
	CWT_UNUSED(ddi);
	return FALSE;
}

BOOL __ddi_recall(CwtDBHandler *dbh, CwtDDI *ddi)
{
	CWT_UNUSED(dbh);
	CWT_UNUSED(ddi);
	return FALSE;
}

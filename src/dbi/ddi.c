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


static inline void __clear_type(CwtDDIColumn *col)
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
	/*CWT_FREE(t);*/
}


static void __ddi_cleanupColumn(void *col)
{
	CwtDDIColumn *c = (CwtDDIColumn*)col;

	if( c->name )
		CWT_FREE(c->name);
	if( c->defaultValue )
		CWT_FREE(c->defaultValue);

	/*CWT_FREE(c);*/
}



CwtDDI* __ddi_createDDI(const CWT_CHAR *name, const CWT_CHAR *charset)
{
	CwtDDI *ddi = CWT_MALLOC(CwtDDI);

	if( !__strNS ) {
		__strNS  = cwtStrNS();
		__listNS = cwtListNS();
	}

	__strNS->bzero(ddi, sizeof(CwtDDI));
	ddi->name = __strNS->strdup(name);
	ddi->charset = __strNS->strdup(charset);
	ddi->tables = __listNS->create(sizeof(CwtDDITable), __ddi_cleanupTable);
	return ddi;
}

void __ddi_freeDDI(CwtDDI *ddi)
{
	CWT_ASSERT(ddi);

	if( ddi->name )
		CWT_FREE(ddi->name);

	if( ddi->charset )
		CWT_FREE(ddi->charset);

	if( ddi->tables )
		__listNS->free(ddi->tables);

	CWT_FREE(ddi);
}

CwtDDITable* __ddi_newTable(CwtDDI *ddi, const CWT_CHAR *name)
{
	CwtDDITable tab;

	CWT_ASSERT(ddi);
	CWT_ASSERT(ddi->tables);

	__strNS->bzero(&tab, sizeof(CwtDDITable));
	tab.name = __strNS->strdup(name);
	tab.columns = __listNS->create(sizeof(CwtDDIColumn), __ddi_cleanupColumn);

	__listNS->append(ddi->tables, &tab);

	return __listNS->last(ddi->tables);
}

CwtDDIColumn* __ddi_newColumn(CwtDDITable *tab, const CWT_CHAR *name)
{
	CwtDDIColumn col;

	CWT_ASSERT(tab);
	CWT_ASSERT(tab->columns);

	__strNS->bzero(&col, sizeof(CwtDDIColumn));
	col.pOwner = tab;
	col.name  = __strNS->strdup(name);

	__listNS->append(tab->columns, &col);

	return __listNS->last(tab->columns);
}


CwtDDITable* __ddi_findTable(CwtDDI *ddi, const CWT_CHAR *name)
{
	CwtListIterator it;

	CWT_ASSERT(ddi);

	if( ddi->tables ) {
		__listNS->begin(ddi->tables, &it);
		while( __listNS->hasMore(&it) ) {
			CwtDDITable *t = __listNS->next(&it);
			if( t && __strNS->streq(name, t->name) ) {
				return t;
			}
		}
	}

	return NULL;
}

CwtDDIColumn* __ddi_findColumn(CwtDDITable *tab, const CWT_CHAR *name)
{
	CwtListIterator it;

	CWT_ASSERT(tab);

	if( tab->columns ) {
		__listNS->begin(tab->columns, &it);
		while( __listNS->hasMore(&it) ) {
			CwtDDIColumn *c = __listNS->next(&it);
			if( c && __strNS->streq(name, c->name) ) {
				return c;
			}
		}
	}

	return NULL;
}

BOOL __ddi_cTypeBool(CwtDDIColumn *col)
{
	__clear_type(col);
	col->type = CwtType_BOOL;
	return TRUE;
}

BOOL __ddi_cTypeInt(CwtDDIColumn *col, LONGLONG min, ULONGLONG max)
{
	__clear_type(col);
	col->type = CwtType_INT;
	col->opts.int_opts.min = min;
	col->opts.int_opts.max = max;
	return TRUE;
}

BOOL __ddi_cTypeFloat(CwtDDIColumn *col, UINT prec, UINT scale)
{
	__clear_type(col);
	col->type = CwtType_FLOAT;
	col->opts.float_opts.prec = prec;
	col->opts.float_opts.scale = scale;
	return TRUE;
}

BOOL __ddi_cTypeText(CwtDDIColumn *col, ULONGLONG maxlen)
{
	__clear_type(col);
	col->type = CwtType_TEXT;
	col->opts.maxlen = maxlen;
	return TRUE;
}

BOOL __ddi_cTypeBlob(CwtDDIColumn *col, ULONGLONG maxlen)
{
	__clear_type(col);
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
	__clear_type(col);

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
	__clear_type(col);

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

BOOL __ddi_cDefault(CwtDDIColumn *col, const CWT_CHAR *defaultValue)
{
	CWT_ASSERT(col);
	if( col->defaultValue )
		CWT_FREE(col->defaultValue);
	col->defaultValue = cwtStrNS()->strdup(defaultValue);
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

BOOL __ddi_cIndex(CwtDDIColumn *col, BOOL yes)
{
	CWT_ASSERT(col);
	col->is_index = yes ? 1 : 0;
	return TRUE;
}


BOOL __ddi_cUniq(CwtDDIColumn *col, BOOL yes)
{
	CWT_ASSERT(col);
	col->is_uniq = yes ? 1 : 0;
	return TRUE;
}

BOOL __ddi_cPK(CwtDDIColumn *col)
{
	CWT_ASSERT(col);
	col->pOwner->pPK = col;
	return TRUE;
}


/* Helper function for __ddi_deploy and __ddi_recall */
static BOOL __ddi_exec_queries(CwtDBHandler *dbh, CwtDDI *ddi, int flags, CwtStrList* (*specFor) (CwtDDI*, int flags))
{
	CwtStrListNS *slNS = cwtStrListNS();

	CwtStrList *spec;
	CwtStrListIterator it;
	BOOL ok = TRUE;

	spec = specFor(ddi, flags);

	if( spec ) {
		slNS->begin(spec, &it);

		while( slNS->hasMore(&it) ) {
			if( !dbh->driver()->query(dbh, slNS->next(&it)) ) {
				ok = FALSE;
				break;
			}
		}

		slNS->free(spec);
	}
	return ok;
}


BOOL __ddi_deploy(CwtDBHandler *dbh, CwtDDI *ddi, int flags)
{
	return __ddi_exec_queries(dbh, ddi, flags, dbh->driver()->specForDeploy);
}

BOOL __ddi_recall(CwtDBHandler *dbh, CwtDDI *ddi, int flags)
{
	return __ddi_exec_queries(dbh, ddi, flags, dbh->driver()->specForRecall);
}

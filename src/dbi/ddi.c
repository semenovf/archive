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



CwtDDI* __ddi_createDDI(void)
{
	CwtDDI *ddi = CWT_MALLOC(CwtDDI);

	if( !__strNS ) {
		__strNS  = cwtStrNS();
		__listNS = cwtListNS();
	}

	__strNS->bzero(ddi, sizeof(CwtDDI));
	ddi->tables = __listNS->create(sizeof(CwtDDITable), __ddi_cleanupTable);
	return ddi;
}

void __ddi_freeDDI(CwtDDI *ddi)
{
	CWT_ASSERT(ddi);
	if( ddi->tables )
		__listNS->free(ddi->tables);
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
	col->name = __strNS->strdup(name);

	__listNS->append(tab->columns, col);
	return col;
}


BOOL  __ddi_cType (CwtDDIColumn *col, CwtDDI_Type type)
{
	CWT_ASSERT(col);
	CWT_ASSERT(col->name);

	if( col->type == CWT_DDI_TYPE_REF ) {
		col->ref = NULL;
	}
	col->type = type;

	return TRUE;
}

BOOL __ddi_cRef (CwtDDIColumn *col, CwtDDITable *tab)
{
	CWT_ASSERT(col);
	CWT_ASSERT(tab);
	CWT_ASSERT(col->name);

	col->type = CWT_DDI_TYPE_REF;
	col->ref = tab;

	return TRUE;
}

BOOL __ddi_cAutoinc (CwtDDIColumn *col, int inc)
{
	CWT_ASSERT(col);
	CWT_ASSERT(col->name);

	if( inc >= 0 ) {
		if( col->type == CWT_DDI_TYPE_INT ) {
			col->autoinc = inc;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL __ddi_cNull (CwtDDIColumn *col, BOOL yes)
{
	CWT_ASSERT(col);
	CWT_ASSERT(col->name);

	if( col->type != CWT_DDI_TYPE_REF ) {
		col->is_null = yes ? 1 : 0;
		return TRUE;
	}

	printf_error(_Tr("column '%s' can not be NULL"), col->name);
	return FALSE;
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

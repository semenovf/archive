/*
 * ddi.c
 *
 *  Created on: 30.07.2012
 *      Author: wladt
 */


#include <cwt/dbi/ddi.h>

typedef struct _CwtDDI {
	CwtList      *tables;
} CwtDDI;

CwtDDI* __ddi_createDDI(void)
{
	CwtDDI *ddi = CWT_MALLOC(CwtDDI);
	cwtStrNS()->bzero(ddi, sizeof(CwtDDI));
	ddi->tables = cwtListNS()->create(sizeof(CwtDDITable*), NULL);
	return ddi;
}

void __ddi_freeDDI(CwtDDI *ddi)
{
	CWT_ASSERT(ddi);
	if( ddi->tables )
		cwtListNS()->free(ddi->tables);
}

CwtDDITable* __ddi_newTable(CwtDDI *ddi, const CWT_CHAR *name)
{
	CwtStrNS  *strNS  = cwtStrNS();
	CwtListNS *listNS = cwtListNS();
	CwtDDITable *tab;

	CWT_ASSERT(ddi);
	CWT_ASSERT(ddi->tables);

	tab = CWT_MALLOC(CwtDDITable);
	strNS->bzero(tab, sizeof(CwtDDITable));
	tab->name = strNS->strdup(name);

	tabNode = CWT_MALLOC(CwtListNode);
	tabNode->data = tab;

	listNS->insertFirst();

	return tab;
}

CwtDDIColumn* __ddi_addColumn(CwtDDITable *table, const CWT_CHAR *name)
{

}

BOOL __ddi_deploy(CwtDBHandler*, CwtDDI *ddi)
{

}

BOOL __ddi_recall(CwtDBHandler*, CwtDDI *ddi)
{

}





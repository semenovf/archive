/*
 * list.c
 *
 *  Created on: 01.08.2012
 *      Author: wladt
 */


#include <stdio.h>
#include <cwt/types.h>
#include <cwt/test.h>
#include <cwt/str.h>
#include <cwt/list.h>

static CwtStrNS *__strNS = NULL;
static CwtListNS *__listNS = NULL;



static void test_00(void)
{
	CwtList *list;
	size_t i;
	size_t count = 10000;

	list = __listNS->create(sizeof(int), NULL);

	i = CWT_INT_MAX;
	__listNS->append(list, &i);
	CWT_TEST_OK( *((int*)__listNS->first(list)) == CWT_INT_MAX);

	i = CWT_INT_MAX - 1;
	__listNS->append(list, &i);
	CWT_TEST_OK( *((int*)__listNS->last(list)) == CWT_INT_MAX - 1);

	i = CWT_INT_MIN;
	__listNS->prepend(list, &i);
	CWT_TEST_OK( *((int*)__listNS->first(list)) == CWT_INT_MIN);

	CWT_TEST_OK(__listNS->size(list) == 3);

	__listNS->clear(list);

	CWT_TEST_OK(__listNS->size(list) == 0);

	for( i = 0; i < count; i++ ) {
		__listNS->append(list, &i);
	}

	CWT_TEST_OK(__listNS->size(list) == count);

	for( i = 0; i < count; i++ ) {
		__listNS->removeFirst(list);
	}

	CWT_TEST_OK(__listNS->size(list) == 0);

	__listNS->free(list);
}




const CWT_CHAR* text_01[] = {
	  _T("Lorem ipsum dolor sit amet, consectetuer adipiscing elit,")
	, _T("sed diam nonummy nibh euismod tincidunt ut laoreet dolore")
	, _T("magna aliquam erat volutpat. Ut wisi enim ad minim veniam,")
	, _T("quis nostrud exerci tation ullamcorper suscipit lobortis")
	, _T("nisl ut aliquip ex ea commodo consequat. Duis autem vel eum")
	, _T("iriure dolor in hendrerit in vulputate velit esse molestie")
	, _T("consequat, vel illum dolore eu feugiat nulla facilisis at")
	, _T("vero eros et accumsan et iusto odio dignissim qui blandit")
	, _T("praesent luptatum zzril delenit augue duis dolore te feugait")
	, _T("nulla facilisi. Nam liber tempor cum soluta nobis eleifend")
	, _T("option congue nihil imperdiet doming id quod mazim placerat")
	, _T("facer possim assum. Typi non habent claritatem insitam; est")
	, _T("usus legentis in iis qui facit eorum claritatem.")
	, _T("Investigationes demonstraverunt lectores legere me lius quod")
	, _T("ii legunt saepius. Claritas est etiam processus dynamicus,")
	, _T("qui sequitur mutationem consuetudium lectorum. Mirum est")
	, _T("notare quam littera gothica, quam nunc putamus parum claram,")
	, _T("anteposuerit litterarum formas humanitatis per seacula quarta")
	, _T("decima et quinta decima. Eodem modo typi, qui nunc nobis")
	, _T("videntur parum clari, fiant sollemnes in futurum.")
};
typedef struct _struct_01
{
	size_t    index;
	CWT_CHAR *text;
} struct_01;

static void cleanup_01(void *data)
{
	struct_01 *d = (struct_01*)data;
	if( d->text ) {
		CWT_TEST_OK(__strNS->strEq(d->text, text_01[d->index]));
		CWT_FREE(d->text);
		d->text = NULL;
	}
}

static void test_01(void)
{
	CwtList *list;
	size_t i;

	list = __listNS->create(sizeof(struct_01), cleanup_01);

	for(i = 0; i < sizeof(text_01)/sizeof(text_01[0]); i++) {
		struct_01 d;
		d.index = i;
		d.text = __strNS->strDup(text_01[i]);
		__listNS->append(list, &d);
	}
	__listNS->free(list);
}

int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	__strNS = cwt_str_ns();
	__listNS = cwt_list_ns();

	CWT_BEGIN_TESTS(27);
	test_00();
	test_01();

	CWT_END_TESTS;
}



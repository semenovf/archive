/*
 * bytearr.c
 *
 *  Created on: 07.07.2012
 *      Author: wladt
 */


#include <cwt/bytearr.h>
#include <cwt/str.h>
#include <string.h>


static CwtByteArrayNS __ba_ns;
static CwtVectorNS    *__vector_ns = NULL;

static void          ba_init          (CwtByteArray *ba)
	{ __vector_ns->init(ba, sizeof(BYTE)); }

static void          ba_init_sized     (CwtByteArray *ba, size_t initial_size, size_t max_capacity)
	{ __vector_ns->initSized(ba, sizeof(BYTE), initial_size, max_capacity); }

static CwtVector*    ba_create        (void)
	{ return __vector_ns->create(sizeof(BYTE)); }

static CwtVector*    ba_create_sized   (size_t initial_size, size_t max_capacity)
	{ return __vector_ns->createSized(sizeof(BYTE), initial_size, max_capacity); }

static BYTE          ba_at            (CwtByteArray *ba, size_t index)
	{ const BYTE *pb; pb = __vector_ns->at(ba, index); return pb ? *pb : 0; }

static BYTE          ba_first         (CwtByteArray *ba)
	{ const BYTE *pb; pb = __vector_ns->constFirst(ba); return pb ? *pb : 0; }

static BYTE          ba_last          (CwtByteArray *ba)
	{ const BYTE *pb; pb = __vector_ns->constLast(ba); return pb ? *pb : 0; }

static BYTE*         ba_data          (CwtByteArray *ba)
	{ return (BYTE*)__vector_ns->data(ba); }

static const BYTE*   ba_const_data    (CwtByteArray *ba)
	{ return (const BYTE*)__vector_ns->constData(ba); }

static void          ba_append        (CwtByteArray *ba, BYTE b)
	{ __vector_ns->append(ba, &b); }

static void          ba_push_back     (CwtByteArray *ba, BYTE b)
	{ __vector_ns->pushBack(ba, &b); }

static void          ba_push_front    (CwtByteArray *ba, BYTE b)
	{ __vector_ns->pushFront(ba, &b); }

static void          ba_insert        (CwtByteArray *ba, BYTE b, size_t pos)
	{ __vector_ns->insert(ba, &b, pos); }

static void          ba_insert_rep    (CwtByteArray *ba, BYTE b, size_t pos, size_t n)
	{ __vector_ns->insertRep(ba, &b, pos, n); }

static void          ba_fill          (CwtByteArray *ba, BYTE b, size_t n)
	{ __vector_ns->fill(ba, &b, n); }

static BOOL          ba_index_of      (CwtByteArray *ba, BYTE b, size_t off_from, size_t *index)
	{ return __vector_ns->indexOf(ba, &b, off_from, index); }

static BOOL          ba_replace       (CwtByteArray *ba, BYTE b, size_t pos)
	{ return __vector_ns->replace(ba, &b, pos); }

static const char*   ba_cstr          (CwtByteArray *ba);
static void          ba_ltrim         (CwtByteArray *ba);
static void          ba_rtrim         (CwtByteArray *ba);
static void          ba_trim          (CwtByteArray *ba);
static void          ba_append_bytes  (CwtByteArray *ba, const BYTE *bytes, size_t n);

static CwtByteArrayNS __ba_ns = {
	  _CWT_VECTOR_COMMON_INIT
	, ba_init
	, ba_init_sized
	, ba_create
	, ba_create_sized
	, ba_at
	, ba_first
	, ba_last
	, ba_data
	, ba_const_data
	, ba_append
	, ba_append_bytes
	, ba_push_back
	, ba_push_front
	, ba_insert
	, ba_insert_rep
	, ba_fill
	, ba_index_of
	, ba_replace
	, ba_cstr
	, ba_ltrim
	, ba_rtrim
	, ba_trim
};


DLL_API_EXPORT CwtByteArrayNS* cwt_bytearray_ns(void)
{
	if (!__vector_ns) {
		__vector_ns = cwt_vector_ns();
		memcpy(&__ba_ns, __vector_ns, _CWT_VECTOR_COMMON_N * sizeof(void*));
	}
	return &__ba_ns;
}

static const char* ba_cstr(CwtByteArray *ba)
{
	CWT_ASSERT(ba);

	__ba_ns.pushBack(ba, '\x0');
	ba->count--;
	return (const char*)__ba_ns.constData(ba);
}

static void ba_ltrim(CwtByteArray *ba)
{
	size_t i;
	const char *pch;

	CWT_ASSERT(ba);

	if (ba->count == 0)
		return;

	i = 0;
	pch = (const char*)__ba_ns.constData(ba);

	while( i < ba->count
			&& ( *pch == ' '
				|| *pch == '\t'
				|| *pch == '\n'
				|| *pch == '\v'
				|| *pch == '\f'
				|| *pch == '\r' ) ) {
		++pch;
		++i;
	}

	__ba_ns.popFront(ba, i);
}

static void ba_rtrim(CwtByteArray *ba)
{
	size_t i;
	const char *pch;

	CWT_ASSERT(ba);

	if (ba->count == 0)
		return;

	i = ba->count;
	pch = (const char*)__ba_ns.constData(ba) + ba->count - 1;

	while( i > 0
			&& (   *pch == ' '
				|| *pch == '\t'
				|| *pch == '\n'
				|| *pch == '\v'
				|| *pch == '\f'
				|| *pch == '\r' )) {
		--pch;
		--i;
	}

	__ba_ns.popBack(ba, ba->count - i);
}

static void ba_trim(CwtByteArray *ba)
{
	CWT_ASSERT(ba);
	ba_ltrim(ba);
	ba_rtrim(ba);
}


static void ba_append_bytes (CwtByteArray *ba, const BYTE *bytes, size_t n)
{
	size_t i;
	BYTE *data;

	CWT_ASSERT(ba);
	CWT_ASSERT(__vector_ns->reserve(ba, __vector_ns->size(ba) + n));

	data = (BYTE*)__vector_ns->data(ba) + ba->count;

	for (i = 0; i < n; ++i)
		*data++ = bytes[i];
	ba->count += n;

}

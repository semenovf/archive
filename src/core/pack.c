/**
 * @file   pack.c
 * @author wladt
 * @date   Nov 12, 2012 9:32:58 AM
 *
 * @brief This file contains functions @c pack and @c unpack
 * 		for pack data into byte array and unpack byte array
 * 		into data according to template string.
 *
 * 		Pack Algorithm               Unpack Algorithm
 * 		----------   ----------      ----------   ----------
 * 		|  data  |   |  tmpl  |      | buffer |   |  tmpl  |
 * 		----------   ----------      ----------   ----------
 * 		        \      /                     \      /
 * 		         \    /                       \    /
 * 		          \  /                         \  /
 * 		           \/                           \/
 *                 |                            |
 * 	               |                            |
 * 		           V                            V
 * 		      ----------                   ----------
 * 		      | buffer |                   |  data  |
 * 		      ----------                   ----------
 *
 */


#include <cwt/utils.h>
#include <cwt/errno.h>
#include <cwt/fsm.h>
#include <cwt/fsm_common.h>
#include <cwt/str.h>

typedef enum _PackItemType {
	  Pack_ItemType_Invalid
	, Pack_ItemType_Quad
	, Pack_ItemType_Long
	, Pack_ItemType_Word
	, Pack_ItemType_Byte
	, Pack_ItemType_Double
	, Pack_ItemType_Float
	, Pack_ItemType_Count
} ItemType;

typedef enum _PackAct {
	  PackAct_Pack
	, PackAct_Unpack
	, PackAct_PackUniType
	, PackAct_UnpackUniType
} PackAct;

typedef struct _PackContext {
	PackAct act;
	int     err;     /* error code, see CwtErrno */

	BYTE  *buf;
	size_t buf_sz;
	size_t buf_off;

	int    item_type;
	int    nrepeat;

	size_t data_count;
	size_t data_index;
	void   **data;
} PackContext;


static void set_error(const void *data, size_t len, void *context, void *action_args);
static void set_item_type(const void *data, size_t len, void *context, void *action_args);
static void set_item_type(const void *data, size_t len, void *context, void *action_args);
static void set_nrepeat(const void *data, size_t len, void *context, void *action_args);
static void do_repeat(const void *data, size_t len, void *context, void *action_args);

/*
 * pack         = seq
 *
 * //unpack       = forward-seq [ backward ]
 *
 * //forward-seq  = *1 seq
 *
 * //backward-seq = "-" seq
 *
 * seq          =  1*repeat_num
 *
 * repeat_num   =  num *1( "[" 1*DIGIT "]" )
 *
 * num          = "q" / "l" / "w" / "b" / "f" / "d"
 *
 */

static int __type_quad   = Pack_ItemType_Quad;
static int __type_long   = Pack_ItemType_Long;
static int __type_word   = Pack_ItemType_Word;
static int __type_byte   = Pack_ItemType_Byte;
static int __type_double = Pack_ItemType_Double;
static int __type_float  = Pack_ItemType_Float;

static int __err_inval   = Cwt_Err_Invalid;

static CwtFsmTransition num_fsm[] = {
	  {-1, 1, FSM_MATCH_STR(_T("q"), 1), FSM_ACCEPT, set_item_type, &__type_quad }
	, {-1, 2, FSM_MATCH_STR(_T("l"), 1), FSM_ACCEPT, set_item_type, &__type_long }
	, {-1, 3, FSM_MATCH_STR(_T("w"), 1), FSM_ACCEPT, set_item_type, &__type_word }
	, {-1, 4, FSM_MATCH_STR(_T("b"), 1), FSM_ACCEPT, set_item_type, &__type_byte }
	, {-1, 5, FSM_MATCH_STR(_T("d"), 1), FSM_ACCEPT, set_item_type, &__type_double }
	, {-1,-1, FSM_MATCH_STR(_T("f"), 1), FSM_ACCEPT, set_item_type, &__type_float }
};

/* *1("[" 1*DIGIT "]" ) */
static CwtFsmRptBounds __dec_1more_rpt = { 1, -1 };
static CwtFsmTransition repeat_fsm[] = {
	  { 1, 3, FSM_MATCH_STR(_T("["), 1),       FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_RPT(DIGIT_FSM, &__dec_1more_rpt), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_STR(_T("]"), 1),       FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_NOTHING,               FSM_ACCEPT, NULL, NULL }
};

/* num *1( "[" 1*DIGIT "]" ) */
static CwtFsmTransition repeat_num_fsm[] = {
	  { 1, 3, FSM_MATCH_FSM(num_fsm),    FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_FSM(repeat_fsm), FSM_NORMAL, set_nrepeat, NULL }
	, {-1,-1, FSM_MATCH_NOTHING,         FSM_ACCEPT, do_repeat, NULL }
	, {-1,-1, FSM_MATCH_NOTHING,         FSM_REJECT, set_error, &__err_inval }
};

/* 1*repeat_num */
static CwtFsmTransition seq_fsm[] = {
	  {1,-1, FSM_MATCH_FSM(repeat_num_fsm), FSM_ACCEPT, NULL, NULL }
	, {1,-1, FSM_MATCH_FSM(repeat_num_fsm), FSM_ACCEPT, NULL, NULL }
};

static CwtFsmTransition *pack_fsm = seq_fsm;
static CwtFsmTransition *unpack_fsm = seq_fsm;


static ssize_t __pack_helper (CwtFsmTransition *trans
	, const CWT_CHAR *template_str
	, BYTE *buf, size_t buf_sz
	, void **data, size_t data_count
	, PackAct act)
{
	CwtFsm fsm;
	PackContext ctx;

	cwt_str_ns()->bzero(&ctx, sizeof(ctx));
	ctx.act        = act;
	ctx.buf        = buf;
	ctx.buf_sz     = buf_sz;
	ctx.data       = data;
	ctx.data_count = data_count;

	FSM_INIT(fsm, CWT_CHAR, trans
				, &ctx
				, cwt_fsm_belong_cwtchar, cwt_fsm_exact_cwtchar, cwt_fsm_range_cwtchar);

	return cwt_fsm_ns()->exec(&fsm, 0, template_str, cwt_str_ns()->strLen(template_str)) < 0
			? (ssize_t)-1
			: ctx.err != Cwt_NoError
			  	  ? (ssize_t)-1
			  	  : (ssize_t)ctx.buf_off;
}

/**
 * @brief Packs data into byte array according to template string.
 *
 * @details Data is represented as a array of pointers to the real variables.
 *
 * @param template_str Template string.
 * @param buf Buffer to store packed data.
 * @param buf_sz Buffer size.
 * @param data Data to be packed.
 * @param data_count Number of elements in the array of pointers @c data
 * @return Total bytes packed.
 */
ssize_t __utils_pack (const CWT_CHAR *template_str
		, BYTE *buf, size_t buf_sz
		, void *data[], size_t data_count )
{
	return __pack_helper (pack_fsm, template_str, buf, buf_sz, data, data_count, PackAct_Pack);
}

/**
 *
 * @param template_str
 * @param buf
 * @param buf_sz
 * @param data
 * @param data_count
 * @return Total bytes unpacked
 */
ssize_t __utils_unpack (const CWT_CHAR *template_str
		, BYTE *buf, size_t buf_sz
		, void *data[], size_t data_count )
{
	return __pack_helper (unpack_fsm, template_str, buf, buf_sz, data, data_count, PackAct_Unpack);
}


ssize_t __utils_packUniType  (const CWT_CHAR *template_str
		, BYTE *buf, size_t buf_sz
		, CwtUniType *data, size_t data_count)
{
	return __pack_helper (unpack_fsm, template_str, buf, buf_sz, (void**)&data, data_count, PackAct_PackUniType);
}

ssize_t __utils_unpackUniType (const CWT_CHAR *template_str
		, BYTE *buf, size_t buf_sz
		, CwtUniType *data, size_t data_count)
{
	return __pack_helper (unpack_fsm, template_str, buf, buf_sz, (void**)&data, data_count, PackAct_UnpackUniType);
}


static void set_error(const void *data, size_t len, void *context, void *action_args)
{
	PackContext *ctx = (PackContext*)context;
	CWT_UNUSED(data);
	if( len > 0 )
		ctx->err = *(int*)action_args;
}


static void set_item_type(const void *data, size_t len, void *context, void *action_args)
{
	PackContext *ctx = (PackContext*)context;
	CWT_UNUSED2(data, len);
	ctx->item_type = *(int*)action_args;
	ctx->nrepeat   = 1;
}

static void set_nrepeat(const void *data, size_t len, void *context, void *action_args)
{
	PackContext *ctx = (PackContext*)context;
	CWT_CHAR *s;
	BOOL ok = FALSE;

	CWT_UNUSED(action_args);

	if( len == 0 ) {
		ctx->nrepeat = 1;
	} else {
		s = cwt_str_ns()->strNdup(((const CWT_CHAR *)data) + 1, len-2);
		ctx->nrepeat = cwt_str_ns()->toINT32(s, 10, &ok);

		if( !ok )
			ctx->nrepeat = -1;

		CWT_FREE(s);
	}
}

static void __int8ToBytes     (void *pnum, BYTE bytes[]) { bytes[0] = (*(BYTE*)pnum); }
static void __int16ToBytes    (void *pnum, BYTE bytes[]) { cwt_utils_ns()->int16ToBytes(*(INT16*)pnum, bytes); }
static void __int32ToBytes    (void *pnum, BYTE bytes[]) { cwt_utils_ns()->int32ToBytes(*(INT32*)pnum, bytes); }
static void __int64ToBytes    (void *pnum, BYTE bytes[]) { cwt_utils_ns()->int64ToBytes(*(INT64*)pnum, bytes); }
static void __floatToBytes    (void *pnum, BYTE bytes[]) { cwt_utils_ns()->floatToBytes(*(float*)pnum, bytes); }
static void __doubleToBytes   (void *pnum, BYTE bytes[]) { cwt_utils_ns()->doubleToBytes(*(double*)pnum, bytes); }

static void __uniTypeInt8ToBytes   (CwtUniType *ut, BYTE bytes[])
	{ bytes[0] = cwt_unitype_ns()->toBYTE(ut, NULL); }
static void __uniTypeInt16ToBytes  (CwtUniType *ut, BYTE bytes[])
	{ cwt_utils_ns()->int16ToBytes((INT16)cwt_unitype_ns()->toSHORT(ut, NULL), bytes); }
static void __uniTypeInt32ToBytes  (CwtUniType *ut, BYTE bytes[])
	{ cwt_utils_ns()->int32ToBytes((INT32)cwt_unitype_ns()->toLONG(ut, NULL), bytes); }
static void __uniTypeInt64ToBytes  (CwtUniType *ut, BYTE bytes[])
	{ cwt_utils_ns()->int64ToBytes((INT64)cwt_unitype_ns()->toLONGLONG(ut, NULL), bytes); }
static void __uniTypeFloatToBytes  (CwtUniType *ut, BYTE bytes[])
	{ cwt_utils_ns()->floatToBytes(cwt_unitype_ns()->toFLOAT(ut, NULL), bytes); }
static void __uniTypeDoubleToBytes (CwtUniType *ut, BYTE bytes[])
	{ cwt_utils_ns()->doubleToBytes(cwt_unitype_ns()->toDOUBLE(ut, NULL), bytes); }

static void __bytesToInt8     (const BYTE bytes[], void *pnum) { *((INT8*)pnum) = (INT8)bytes[0]; }
static void __bytesToInt16    (const BYTE bytes[], void *pnum) { *((INT16*)pnum) = cwt_utils_ns()->bytesToInt16(bytes); }
static void __bytesToInt32    (const BYTE bytes[], void *pnum) { *((INT32*)pnum) = cwt_utils_ns()->bytesToInt32(bytes); }
static void __bytesToInt64    (const BYTE bytes[], void *pnum) { *((INT64*)pnum) = cwt_utils_ns()->bytesToInt64(bytes); }
static void __bytesToFloat    (const BYTE bytes[], void *pnum) { *((float*)pnum) = cwt_utils_ns()->bytesToFloat(bytes); }
static void __bytesToDouble   (const BYTE bytes[], void *pnum) { *((double*)pnum) = cwt_utils_ns()->bytesToDouble(bytes); }

static void __bytesToUniTypeInt8     (const BYTE bytes[], CwtUniType *ut)
	{ cwt_unitype_ns()->setSBYTE(ut, (INT8)bytes[0]); }
static void __bytesToUniTypeInt16    (const BYTE bytes[], CwtUniType *ut)
	{ cwt_unitype_ns()->setSHORT(ut, (SHORT)cwt_utils_ns()->bytesToInt16(bytes)); }
static void __bytesToUniTypeInt32    (const BYTE bytes[], CwtUniType *ut)
	{ cwt_unitype_ns()->setLONG(ut, (LONG)cwt_utils_ns()->bytesToInt32(bytes)); }
static void __bytesToUniTypeInt64    (const BYTE bytes[], CwtUniType *ut)
	{ cwt_unitype_ns()->setLONGLONG(ut, (LONGLONG)cwt_utils_ns()->bytesToInt64(bytes)); }
static void __bytesToUniTypeFloat    (const BYTE bytes[], CwtUniType *ut)
	{ cwt_unitype_ns()->setFLOAT(ut, cwt_utils_ns()->bytesToFloat(bytes)); }
static void __bytesToUniTypeDouble   (const BYTE bytes[], CwtUniType *ut)
	{ cwt_unitype_ns()->setDOUBLE(ut, cwt_utils_ns()->bytesToDouble(bytes)); }


typedef struct _PackData {
	int type_size;
	void (*numToBytes)(void *pnum, BYTE bytes[]);
	void (*bytesToNum)(const BYTE bytes[], void *pnum);
	void (*uniTypeToBytes)(CwtUniType *ut, BYTE bytes[]);
	void (*bytesToUniType)(const BYTE bytes[], CwtUniType *ut);
} PackData;

static PackData __pack_data[] = {
	  {-1, NULL, NULL, NULL, NULL }
	, { sizeof(INT64),  __int64ToBytes,  __bytesToInt64 , __uniTypeInt64ToBytes,  __bytesToUniTypeInt64 }
	, { sizeof(INT32),  __int32ToBytes,  __bytesToInt32 , __uniTypeInt32ToBytes,  __bytesToUniTypeInt32 }
	, { sizeof(INT16),  __int16ToBytes,  __bytesToInt16 , __uniTypeInt16ToBytes,  __bytesToUniTypeInt16 }
	, { sizeof(INT8),   __int8ToBytes,   __bytesToInt8  , __uniTypeInt8ToBytes,   __bytesToUniTypeInt8  }
	, { sizeof(double), __doubleToBytes, __bytesToDouble, __uniTypeDoubleToBytes, __bytesToUniTypeDouble}
	, { sizeof(float),  __floatToBytes,  __bytesToFloat , __uniTypeFloatToBytes,  __bytesToUniTypeFloat }
};

static void do_repeat(const void *data, size_t len, void *context, void *action_args)
{
	PackContext *ctx = (PackContext*)context;
	char *ptr;
	char *uptr;

	CWT_UNUSED3(data, len, action_args);

	CWT_ASSERT(ctx->item_type > Pack_ItemType_Invalid && ctx->item_type <= Pack_ItemType_Count);

	if( ctx->data_index == ctx->data_count ) {
		ctx->err = Cwt_Err_Overflow;
		ctx->data_index = ctx->data_count;
		return;
	}

	ptr  = ((char**)ctx->data)[ctx->data_index];
	uptr = *((char**)ctx->data) + ctx->data_index * sizeof(CwtUniType);

	while( ctx->nrepeat-- > 0 ) {

		if( ctx->buf_off + __pack_data[ctx->item_type].type_size > ctx->buf_sz ) {
			ctx->err = Cwt_Err_Overflow;
			ctx->buf_off = ctx->buf_sz;
			break;
		}

		switch( ctx->act ) {
		case PackAct_Pack:
			__pack_data[ctx->item_type].numToBytes(ptr, ctx->buf + ctx->buf_off);
			ptr += __pack_data[ctx->item_type].type_size;
			break;
		case PackAct_Unpack:
			__pack_data[ctx->item_type].bytesToNum(ctx->buf + ctx->buf_off, ptr);
			ptr += __pack_data[ctx->item_type].type_size;
			break;
		case PackAct_PackUniType:
			__pack_data[ctx->item_type].uniTypeToBytes((CwtUniType*)uptr, ctx->buf + ctx->buf_off);
			uptr += sizeof(CwtUniType);
			ctx->data_index++; /* step to next element */
			break;
		case PackAct_UnpackUniType:
			__pack_data[ctx->item_type].bytesToUniType(ctx->buf + ctx->buf_off, (CwtUniType*)uptr);
			uptr += sizeof(CwtUniType);
			ctx->data_index++; /* step to next element */
			break;
		}
		ctx->buf_off += __pack_data[ctx->item_type].type_size;
		CWT_ASSERT(ctx->buf_off <= CWT_SSIZE_T_MAX);
	}

	if( ctx->act == PackAct_Pack || ctx->act == PackAct_Unpack)
		ctx->data_index++; /* step to next pointer */
}


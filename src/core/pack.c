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

typedef enum _ItemType {
	  ItemType_Invalid
	, ItemType_Quad
	, ItemType_Long
	, ItemType_Short
	, ItemType_Char
	, ItemType_Double
	, ItemType_Float
	, ItemType_Count
} ItemType;

typedef struct _PackContext {
	BOOL   is_unpack;
	int    err;     /* error code, see CwtErrno */

	BYTE  *buf;
	size_t buf_sz;
	size_t buf_off;

	int    item_type;
	int    nrepeat;

	size_t data_count;
	size_t data_index;
	void   **data;
} PackContext;


static void set_item_type(const void *data, size_t len, void *context, void *action_args);
static void set_nrepeat(const void *data, size_t len, void *context, void *action_args);
static void do_repeat(const void *data, size_t len, void *context, void *action_args);

/*
 * pack        =  1*repeat_num
 *
 * repeat_num  =  num ( "[" 1*DIGIT "]" )
 *
 * num         = "q" / "l" / "s" / "c" / "f" / "d"
 *
 *
 */

static int __type_quad   = ItemType_Quad;
static int __type_long   = ItemType_Long;
static int __type_short  = ItemType_Short;
static int __type_char   = ItemType_Char;
static int __type_double = ItemType_Double;
static int __type_float  = ItemType_Float;

static CwtFsmTransition num_fsm[] = {
	  {-1, 1, FSM_MATCH_STR(_T("q"), 1), FSM_ACCEPT, set_item_type, &__type_quad }
	, {-1, 2, FSM_MATCH_STR(_T("l"), 1), FSM_ACCEPT, set_item_type, &__type_long }
	, {-1, 3, FSM_MATCH_STR(_T("s"), 1), FSM_ACCEPT, set_item_type, &__type_short }
	, {-1, 4, FSM_MATCH_STR(_T("c"), 1), FSM_ACCEPT, set_item_type, &__type_char }
	, {-1, 5, FSM_MATCH_STR(_T("d"), 1), FSM_ACCEPT, set_item_type, &__type_double }
	, {-1,-1, FSM_MATCH_STR(_T("f"), 1), FSM_ACCEPT, set_item_type, &__type_float }
};

/* "[" 1*DIGIT "]" */
static CwtFsmRepetitionContext __dec_1more_rpt = { DIGIT_FSM, 1, -1 };
static CwtFsmTransition repeat_fsm[] = {
	  { 1,-1, FSM_MATCH_STR(_T("["), 1),       FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_RPT(&__dec_1more_rpt), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_STR(_T("]"), 1),       FSM_ACCEPT, set_nrepeat, NULL }
};

static CwtFsmTransition repeat_num_fsm[] = {
	  { 1,-1, FSM_MATCH_FSM(num_fsm),    FSM_NORMAL, NULL, NULL }
	, { 2, 2, FSM_MATCH_FSM(repeat_fsm), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_NOTHING,         FSM_ACCEPT, do_repeat, NULL }
};

static CwtFsmTransition pack_fsm[] = {
	  {1,-1, FSM_MATCH_FSM(repeat_num_fsm), FSM_ACCEPT, NULL, NULL }
	, {1,-1, FSM_MATCH_FSM(repeat_num_fsm), FSM_ACCEPT, NULL, NULL }
};

static CwtFsmTransition unpack_fsm[] = {
	  {1,-1, FSM_MATCH_FSM(repeat_num_fsm), FSM_ACCEPT, NULL, NULL }
	, {1,-1, FSM_MATCH_FSM(repeat_num_fsm), FSM_ACCEPT, NULL, NULL }
};



static BOOL  __pack_helper (CwtFsmTransition *trans
	, const CWT_CHAR *template_str
	, BYTE *buf, size_t buf_sz
	, void *data[], size_t data_count
	, BOOL is_unpack)
{
	CwtFsm fsm;
	PackContext ctx;

	cwtStrNS()->bzero(&ctx, sizeof(ctx));
	ctx.is_unpack  = is_unpack;
	ctx.buf        = buf;
	ctx.buf_sz     = buf_sz;
	ctx.data       = data;
	ctx.data_count = data_count;

	FSM_INIT(fsm, CWT_CHAR, trans
				, &ctx
				, cwtBelongCwtChar, cwtExactCwtChar, cwtRangeCwtChar);

	return cwtFsmNS()->exec(&fsm, 0, template_str, cwtStrNS()->strLen(template_str)) < 0
			? FALSE
			: ctx.err != Cwt_NoError ? FALSE : TRUE;
}

/**
 * @brief
 *
 * @details
 *
 * @param template_str
 * @param data
 * @param sz data size in bytes
 */
BOOL  __utils_pack (const CWT_CHAR *template_str
		, BYTE *buf, size_t buf_sz
		, void *data[], size_t data_count )
{
	return __pack_helper (pack_fsm, template_str, buf, buf_sz, data, data_count, FALSE);
}

BOOL  __utils_unpack (const CWT_CHAR *template_str
		, BYTE *buf, size_t buf_sz
		, void *data[], size_t data_count )
{
	return __pack_helper (unpack_fsm, template_str, buf, buf_sz, data, data_count, TRUE);
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

	s = cwtStrNS()->strNdup(((const CWT_CHAR *)data) + 1, len-2);
	ctx->nrepeat = cwtStrNS()->toINT32(s, 10, &ok);

	if( !ok )
		ctx->nrepeat = -1;

	CWT_FREE(s);
}

static void __int8ToBytes     (void *pnum, BYTE bytes[]) { bytes[0] = (*(BYTE*)pnum); }
static void __int16ToBytes    (void *pnum, BYTE bytes[]) { cwtUtilsNS()->int16ToBytes(*(INT16*)pnum, bytes); }
static void __int32ToBytes    (void *pnum, BYTE bytes[]) { cwtUtilsNS()->int32ToBytes(*(INT32*)pnum, bytes); }
static void __int64ToBytes    (void *pnum, BYTE bytes[]) { cwtUtilsNS()->int64ToBytes(*(INT64*)pnum, bytes); }
static void __floatToBytes    (void *pnum, BYTE bytes[]) { cwtUtilsNS()->floatToBytes(*(float*)pnum, bytes); }
static void __doubleToBytes   (void *pnum, BYTE bytes[]) { cwtUtilsNS()->doubleToBytes(*(double*)pnum, bytes); }

static void __bytesToInt8     (const BYTE bytes[], void *pnum) { *((INT8*)pnum) = (INT8)bytes[0]; }
static void __bytesToInt16    (const BYTE bytes[], void *pnum) { *((INT16*)pnum) = cwtUtilsNS()->bytesToInt16(bytes); }
static void __bytesToInt32    (const BYTE bytes[], void *pnum) { *((INT32*)pnum) = cwtUtilsNS()->bytesToInt32(bytes); }
static void __bytesToInt64    (const BYTE bytes[], void *pnum) { *((INT64*)pnum) = cwtUtilsNS()->bytesToInt64(bytes); }
static void __bytesToFloat    (const BYTE bytes[], void *pnum) { *((float*)pnum) = cwtUtilsNS()->bytesToFloat(bytes); }
static void __bytesToDouble   (const BYTE bytes[], void *pnum) { *((double*)pnum) = cwtUtilsNS()->bytesToDouble(bytes); }

typedef struct _PackData {
	int type_size;
	void (*numToBytes)(void *pnum, BYTE bytes[]);
	void (*bytesToNum)(const BYTE bytes[], void *pnum);
} PackData;

static PackData __packData[] = {
	  {-1, NULL, NULL }
	, { 8, __int64ToBytes,     __bytesToInt64 }
	, { 4, __int32ToBytes,     __bytesToInt32 }
	, { 2, __int16ToBytes,     __bytesToInt16 }
	, { 1, __int8ToBytes,      __bytesToInt8 }
	, { sizeof(double), __doubleToBytes, __bytesToDouble }
	, { 4, __floatToBytes,     __bytesToFloat }
};

static void do_repeat(const void *data, size_t len, void *context, void *action_args)
{
	PackContext *ctx = (PackContext*)context;

	CWT_UNUSED3(data, len, action_args);

	CWT_ASSERT(ctx->item_type > ItemType_Invalid && ctx->item_type <= ItemType_Count );

	while( ctx->nrepeat-- > 0 ) {

		if( ctx->buf_off + __packData[ctx->item_type].type_size > ctx->buf_sz ) {
			ctx->err = Cwt_Err_Overflow;
			ctx->buf_off = ctx->buf_sz;
			break;
		}

		if( ctx->data_index == ctx->data_count ) {
			ctx->err = Cwt_Err_Overflow;
			ctx->data_index = ctx->data_count;
			break;
		}

		if( ctx->is_unpack ) {
			__packData[ctx->item_type].bytesToNum(ctx->buf + ctx->buf_off, ctx->data[ctx->data_index]);
		} else  {
			__packData[ctx->item_type].numToBytes(ctx->data[ctx->data_index], ctx->buf + ctx->buf_off);
		}
		ctx->buf_off += __packData[ctx->item_type].type_size;
		ctx->data_index++;
	}
}


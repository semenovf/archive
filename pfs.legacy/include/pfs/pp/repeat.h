#ifndef __PFS_PP_REPEAT_H__
#define __PFS_PP_REPEAT_H__

#include <pfs/pp/dec.h>
#include <pfs/pp/inc.h>
#include <pfs/pp/utility.h>

#ifndef PFS_COMMA
#	define PFS_COMMA ,
#endif

#define PFS_ARGS_0(pfx)
#define PFS_ARGS_1(pfx)  PFS_CAT(pfx,1)
#define PFS_ARGS_2(pfx)  PFS_ARGS_1(pfx)  PFS_COMMA PFS_CAT(pfx,2)
#define PFS_ARGS_3(pfx)  PFS_ARGS_2(pfx)  PFS_COMMA PFS_CAT(pfx,3)
#define PFS_ARGS_4(pfx)  PFS_ARGS_3(pfx)  PFS_COMMA PFS_CAT(pfx,4)
#define PFS_ARGS_5(pfx)  PFS_ARGS_4(pfx)  PFS_COMMA PFS_CAT(pfx,5)
#define PFS_ARGS_6(pfx)  PFS_ARGS_5(pfx)  PFS_COMMA PFS_CAT(pfx,6)
#define PFS_ARGS_7(pfx)  PFS_ARGS_6(pfx)  PFS_COMMA PFS_CAT(pfx,7)
#define PFS_ARGS_8(pfx)  PFS_ARGS_7(pfx)  PFS_COMMA PFS_CAT(pfx,8)
#define PFS_ARGS_9(pfx)  PFS_ARGS_8(pfx)  PFS_COMMA PFS_CAT(pfx,9)
#define PFS_ARGS_10(pfx) PFS_ARGS_9(pfx)  PFS_COMMA PFS_CAT(pfx,10)
#define PFS_ARGS_11(pfx) PFS_ARGS_10(pfx) PFS_COMMA PFS_CAT(pfx,11)
#define PFS_ARGS_12(pfx) PFS_ARGS_11(pfx) PFS_COMMA PFS_CAT(pfx,12)
#define PFS_ARGS_13(pfx) PFS_ARGS_12(pfx) PFS_COMMA PFS_CAT(pfx,13)
#define PFS_ARGS_14(pfx) PFS_ARGS_13(pfx) PFS_COMMA PFS_CAT(pfx,14)
#define PFS_ARGS_15(pfx) PFS_ARGS_14(pfx) PFS_COMMA PFS_CAT(pfx,15)
#define PFS_ARGS_16(pfx) PFS_ARGS_15(pfx) PFS_COMMA PFS_CAT(pfx,16)
#define PFS_ARGS_17(pfx) PFS_ARGS_16(pfx) PFS_COMMA PFS_CAT(pfx,17)
#define PFS_ARGS_18(pfx) PFS_ARGS_17(pfx) PFS_COMMA PFS_CAT(pfx,18)
#define PFS_ARGS_19(pfx) PFS_ARGS_18(pfx) PFS_COMMA PFS_CAT(pfx,19)
#define PFS_ARGS_20(pfx) PFS_ARGS_19(pfx) PFS_COMMA PFS_CAT(pfx,20)

#define PFS_PARMS_0(pfx,pfa)
#define PFS_PARMS_1(pfx,pfa)  PFS_CAT(pfx,1) PFS_CAT(pfa,1)
#define PFS_PARMS_2(pfx,pfa)  PFS_PARMS_1(pfx,pfa)  PFS_COMMA PFS_CAT(pfx,2) PFS_CAT(pfa,2)
#define PFS_PARMS_3(pfx,pfa)  PFS_PARMS_2(pfx,pfa)  PFS_COMMA PFS_CAT(pfx,3) PFS_CAT(pfa,3)
#define PFS_PARMS_4(pfx,pfa)  PFS_PARMS_3(pfx,pfa)  PFS_COMMA PFS_CAT(pfx,4) PFS_CAT(pfa,4)
#define PFS_PARMS_5(pfx,pfa)  PFS_PARMS_4(pfx,pfa)  PFS_COMMA PFS_CAT(pfx,5) PFS_CAT(pfa,5)
#define PFS_PARMS_6(pfx,pfa)  PFS_PARMS_5(pfx,pfa)  PFS_COMMA PFS_CAT(pfx,6) PFS_CAT(pfa,6)
#define PFS_PARMS_7(pfx,pfa)  PFS_PARMS_6(pfx,pfa)  PFS_COMMA PFS_CAT(pfx,7) PFS_CAT(pfa,7)
#define PFS_PARMS_8(pfx,pfa)  PFS_PARMS_7(pfx,pfa)  PFS_COMMA PFS_CAT(pfx,8) PFS_CAT(pfa,8)
#define PFS_PARMS_9(pfx,pfa)  PFS_PARMS_8(pfx,pfa)  PFS_COMMA PFS_CAT(pfx,9) PFS_CAT(pfa,9)
#define PFS_PARMS_10(pfx,pfa) PFS_PARMS_9(pfx,pfa)  PFS_COMMA PFS_CAT(pfx,10) PFS_CAT(pfa,10)
#define PFS_PARMS_11(pfx,pfa) PFS_PARMS_10(pfx,pfa) PFS_COMMA PFS_CAT(pfx,11) PFS_CAT(pfa,11)
#define PFS_PARMS_12(pfx,pfa) PFS_PARMS_11(pfx,pfa) PFS_COMMA PFS_CAT(pfx,12) PFS_CAT(pfa,12)
#define PFS_PARMS_13(pfx,pfa) PFS_PARMS_12(pfx,pfa) PFS_COMMA PFS_CAT(pfx,13) PFS_CAT(pfa,13)
#define PFS_PARMS_14(pfx,pfa) PFS_PARMS_13(pfx,pfa) PFS_COMMA PFS_CAT(pfx,14) PFS_CAT(pfa,14)
#define PFS_PARMS_15(pfx,pfa) PFS_PARMS_14(pfx,pfa) PFS_COMMA PFS_CAT(pfx,15) PFS_CAT(pfa,15)
#define PFS_PARMS_16(pfx,pfa) PFS_PARMS_15(pfx,pfa) PFS_COMMA PFS_CAT(pfx,16) PFS_CAT(pfa,16)
#define PFS_PARMS_17(pfx,pfa) PFS_PARMS_16(pfx,pfa) PFS_COMMA PFS_CAT(pfx,17) PFS_CAT(pfa,17)
#define PFS_PARMS_18(pfx,pfa) PFS_PARMS_17(pfx,pfa) PFS_COMMA PFS_CAT(pfx,18) PFS_CAT(pfa,18)
#define PFS_PARMS_19(pfx,pfa) PFS_PARMS_18(pfx,pfa) PFS_COMMA PFS_CAT(pfx,19) PFS_CAT(pfa,19)
#define PFS_PARMS_20(pfx,pfa) PFS_PARMS_19(pfx,pfa) PFS_COMMA PFS_CAT(pfx,20) PFS_CAT(pfa,20)

/* Emulates right shift by 1 position */
/* TODO need more flexible approach */
#define PFS_ARGS_SHIFT1_0(pfx)
#define PFS_ARGS_SHIFT1_1(pfx)
#define PFS_ARGS_SHIFT1_2(pfx)  PFS_CAT(pfx,2)
#define PFS_ARGS_SHIFT1_3(pfx)  PFS_ARGS_SHIFT1_2(pfx)  PFS_COMMA PFS_CAT(pfx,3)
#define PFS_ARGS_SHIFT1_4(pfx)  PFS_ARGS_SHIFT1_3(pfx)  PFS_COMMA PFS_CAT(pfx,4)
#define PFS_ARGS_SHIFT1_5(pfx)  PFS_ARGS_SHIFT1_4(pfx)  PFS_COMMA PFS_CAT(pfx,5)
#define PFS_ARGS_SHIFT1_6(pfx)  PFS_ARGS_SHIFT1_5(pfx)  PFS_COMMA PFS_CAT(pfx,6)
#define PFS_ARGS_SHIFT1_7(pfx)  PFS_ARGS_SHIFT1_6(pfx)  PFS_COMMA PFS_CAT(pfx,7)
#define PFS_ARGS_SHIFT1_8(pfx)  PFS_ARGS_SHIFT1_7(pfx)  PFS_COMMA PFS_CAT(pfx,8)
#define PFS_ARGS_SHIFT1_9(pfx)  PFS_ARGS_SHIFT1_8(pfx)  PFS_COMMA PFS_CAT(pfx,9)
#define PFS_ARGS_SHIFT1_10(pfx) PFS_ARGS_SHIFT1_9(pfx)  PFS_COMMA PFS_CAT(pfx,10)
#define PFS_ARGS_SHIFT1_11(pfx) PFS_ARGS_SHIFT1_10(pfx) PFS_COMMA PFS_CAT(pfx,11)
#define PFS_ARGS_SHIFT1_12(pfx) PFS_ARGS_SHIFT1_11(pfx) PFS_COMMA PFS_CAT(pfx,12)
#define PFS_ARGS_SHIFT1_13(pfx) PFS_ARGS_SHIFT1_12(pfx) PFS_COMMA PFS_CAT(pfx,13)
#define PFS_ARGS_SHIFT1_14(pfx) PFS_ARGS_SHIFT1_13(pfx) PFS_COMMA PFS_CAT(pfx,14)
#define PFS_ARGS_SHIFT1_15(pfx) PFS_ARGS_SHIFT1_14(pfx) PFS_COMMA PFS_CAT(pfx,15)
#define PFS_ARGS_SHIFT1_16(pfx) PFS_ARGS_SHIFT1_15(pfx) PFS_COMMA PFS_CAT(pfx,16)
#define PFS_ARGS_SHIFT1_17(pfx) PFS_ARGS_SHIFT1_16(pfx) PFS_COMMA PFS_CAT(pfx,17)
#define PFS_ARGS_SHIFT1_18(pfx) PFS_ARGS_SHIFT1_17(pfx) PFS_COMMA PFS_CAT(pfx,18)
#define PFS_ARGS_SHIFT1_19(pfx) PFS_ARGS_SHIFT1_18(pfx) PFS_COMMA PFS_CAT(pfx,19)
#define PFS_ARGS_SHIFT1_20(pfx) PFS_ARGS_SHIFT1_19(pfx) PFS_COMMA PFS_CAT(pfx,20)

#define PFS_ARGS_CAT(n,pfx)         PFS_ARGS_ ## n(pfx)
#define PFS_ARGS_SHIFT1_CAT(n,pfx)  PFS_ARGS_SHIFT1_ ## n(pfx)
#define PFS_PARMS_CAT(n,pfx,pfa)    PFS_PARMS_ ## n(pfx,pfa)

#define PFS_JOIN_ARGS(n,pfx)          PFS_ARGS_CAT(n,pfx)
#define PFS_JOIN_ARGS_SHIFT1(n,pfx)   PFS_ARGS_SHIFT1_CAT(n,pfx)
#define PFS_JOIN_PARMS(n,pfx,pfa)     PFS_PARMS_CAT(n,pfx,pfa)
#define PFS_JOIN_TEMPLATE_ARGS(n,pfx) PFS_JOIN_ARGS(n,typename pfx)

/*
PFS_DEC(6) -> 5
PFS_INC(4) -> 5
*/

/*
PFS_JOIN_ARGS(3,_A)          -> _A1, _A2, _A3
PFS_JOIN_PARMS(3,_A,_a)      -> _A1 a1, _A2 a2, _A3 a3
PFS_JOIN_TEMPLATE_ARGS(3,_A) -> typename _A1, typename _A2, _typename _A3
*/

/*
PFS_JOIN_ARGS_SHIFT1(5,_a)   -> _a2 , _a3 , _a4 , _a5
*/

#endif

/**
 * @file   errno.h
 * @author wladt
 * @date   Nov 12, 2012 2:07:22 PM
 *
 * @brief Error codes
 */

#ifndef __CWT_ERRNO_H__
#define __CWT_ERRNO_H__

typedef enum _CwtErrno {
      Cwt_NoError
    , Cwt_Err_IO           /* input/output error */
    , Cwt_Err_Perm         /* operation not permitted */
    , Cwt_Err_Auth         /* authorization denied */
    , Cwt_Err_Interrupt    /* interrupted function call */
	, Cwt_Err_Overflow
	, Cwt_Err_Range
	, Cwt_Err_Invalid      /* invalid argument */
	, Cwt_Err_BadVal       /* value does not match the type or value is out of acceptable range */
	, Cwt_Err_Busy         /* Device or resource busy */
	, Cwt_Err_DBI          /* DBI specific errors */
	, Cwt_Err_Stub = 10000 /* is not categorized yet */
} CwtErrno;

#endif /* __CWT_ERRNO_H__ */

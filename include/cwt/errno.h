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
	, Cwt_Err_Overflow
	, Cwt_Err_Invalid   /* invalid argument */
} CwtErrno;

#endif /* __CWT_ERRNO_H__ */

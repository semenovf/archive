/**
 * @file   effort.h
 * @author wladt
 * @date   Nov 28, 2012 4:36:53 PM
 *
 * @brief
 */

#ifndef __CWT_EFFORT_H__
#define __CWT_EFFORT_H__


typedef struct _CwtEffortBase {
	BOOL (*hasMore)(struct _CwtEffortBase *effort);
	BOOL (*exec)(struct _CwtEffortBase *effort);
} CwtEffortBase;


#endif /* __CWT_EFFORT_H__ */

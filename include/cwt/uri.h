/**
 * @file   uri.h
 * @author wladt
 * @date   Oct 16, 2012 9:12:15 AM
 *
 * @brief
 */

#ifndef __CWT_URI_H__
#define __CWT_URI_H__

#include <cwt/types.h>

typedef struct _CwtUri {
	CWT_CHAR *scheme;
	CWT_CHAR *userinfo;
	CWT_CHAR *host;
	UINT16    port;
	BOOL      host_is_ip;
	CWT_CHAR *path;
	CWT_CHAR *query;
	CWT_CHAR *fragment;
} CwtUri;

typedef struct _CwtUriNS {
    CwtUri*   (*create)  (void);
    void      (*free)    (CwtUri *uri);
	void      (*init)    (CwtUri *uri);
	void      (*destroy) (CwtUri *uri);
	ssize_t   (*parse)   (const CWT_CHAR *uri_string, CwtUri *uri);
	CWT_CHAR* (*compose) (CwtUri *uri);
} CwtUriNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtUriNS* cwtUriNS(void);
EXTERN_C_END


#endif /* __CWT_URI_H__ */

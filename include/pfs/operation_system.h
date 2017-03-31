/* 
 * File:   operationsystem.h
 * Author: wladt
 *
 * Created on March 31, 2017, 6:59 PM
 */

#ifndef __PFS_SYS_OPERATIONSYSTEM_H__
#define __PFS_SYS_OPERATIONSYSTEM_H__

#ifdef __cplusplus
extern "C" {
#endif

#define OS_TYPE_LINUX_S   "linux"
#define OS_TYPE_BSD_S     "bsd"
#define OS_TYPE_MACOS_S   "macos"
#define OS_TYPE_SOLARIS_S "solaris"
#define OS_TYPE_HPUX_S    "hpux"
#define OS_TYPE_MSWIN_S   "mswin"
#define OS_TYPE_CYGWIN_S  "cygwin"
#define OS_TYPE_DOS_S     "dos"
#define OS_TYPE_UNKNOWN_S "unknown"

#define OS_FAMILY_UNIX_S    "unix"
#define OS_FAMILY_MSWIN_S   "mswin"
#define OS_FAMILY_DOS_S     "dos"
#define OS_FAMILY_UNKNOWN_S "unknown"
    
typedef struct operation_system
{
    void (* destroy) (struct operation_system * self);
    int (* descover) (struct operation_system * self);
    char const * (* type) (struct operation_system * self);
    char const * (* family) (struct operation_system * self);
} operation_system_t;

operation_system_t * operation_system_create ();

#ifdef __cplusplus
}
#endif

#endif /* __PFS_SYS_OPERATIONSYSTEM_H__ */
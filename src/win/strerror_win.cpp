/**
 * @file   strerror_win32.cpp
 * @author wladt
 * @date   Feb 15, 2013 10:33:47 AM
 *
 * @brief
 */

#include <string.h>
#include <windows.h>
#include <pfs/string.hpp>

pfs::string pfs_strerror (int errn)
{
	LPWSTR lpMsgBuf;

    DWORD n = FormatMessage(
          FORMAT_MESSAGE_ALLOCATE_BUFFER
        | FORMAT_MESSAGE_FROM_SYSTEM
        | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        (DWORD)errn,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR) & lpMsgBuf,
        0, NULL );

    PFS_ASSERT(n > 0);
    pfs::string errstr(pfs::string::fromUtf16(lpMsgBuf, n));
    LocalFree(lpMsgBuf);
    return errstr;
}

/**
 * @file   strerror_win32.cpp
 * @author wladt
 * @date   Feb 15, 2013 10:33:47 AM
 *
 * @brief
 */

#include <cwt/string.hpp>
#include <windows.h>
#include <strsafe.h>

CWT_NS_BEGIN

String strerror_win32(uint_t errn)
{
    LPVOID lpMsgBuf;

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        (DWORD)errn,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    String emsg((Char*)lpMsgBuf);
    LocalFree(lpMsgBuf);
    return emsg;
}

CWT_NS_END



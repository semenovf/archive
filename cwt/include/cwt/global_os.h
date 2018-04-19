/**
 * @file   global_os.h
 * @author wladt
 * @date   Nov 9, 2012 9:29:58 AM
 *
 * @brief This file based on source from Qt Project
 */

#ifndef __CWT_GLOBAL_OS_H__
#define __CWT_GLOBAL_OS_H__

/*
   The operating system, must be one of: (CWT_OS_x)
     DARWIN   - Darwin OS (synonym for CWT_OS_MAC)
     SYMBIAN  - Symbian
     MSDOS    - MS-DOS and Windows
     OS2      - OS/2
     OS2EMX   - XFree86 on OS/2 (not PM)
     WIN32    - Win32 (Windows 2000/XP/Vista/7 and Windows Server 2003/2008)
     WINCE    - WinCE (Windows CE 5.0)
     CYGWIN   - Cygwin
     SOLARIS  - Sun Solaris
     HPUX     - HP-UX
     ULTRIX   - DEC Ultrix
     LINUX    - Linux
     FREEBSD  - FreeBSD
     NETBSD   - NetBSD
     OPENBSD  - OpenBSD
     BSDI     - BSD/OS
     IRIX     - SGI Irix
     OSF      - HP Tru64 UNIX
     SCO      - SCO OpenServer 5
     UNIXWARE - UnixWare 7, Open UNIX 8
     AIX      - AIX
     HURD     - GNU Hurd
     DGUX     - DG/UX
     RELIANT  - Reliant UNIX
     DYNIX    - DYNIX/ptx
     QNX      - QNX
     LYNX     - LynxOS
     BSD4     - Any BSD 4.4 system
     UNIX     - Any UNIX BSD/SYSV system
*/

#if defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))
#  	define CWT_OS_DARWIN
#  	define CWT_OS_BSD4
#  	ifdef __LP64__
#   	define CWT_OS_DARWIN64
#  	else
#   	define CWT_OS_DARWIN32
#	endif
#elif defined(__SYMBIAN32__) || defined(SYMBIAN)
#	define CWT_OS_SYMBIAN
#	define CWT_NO_POSIX_SIGNALS
#	define CWT_NO_GETIFADDRS
#elif defined(__CYGWIN__)
#	define CWT_OS_CYGWIN
#elif defined(MSDOS) || defined(__MSDOS__) || defined(_MSDOS) /* || defined(__DOS__) */
#	define CWT_OS_MSDOS
#elif defined(__OS2__)
#	if defined(__EMX__)
#   	define CWT_OS_OS2EMX
#	else
#		define CWT_OS_OS2
#	endif
#elif !defined(SAG_COM) && (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))
#	define CWT_OS_WIN32
#	define CWT_OS_WIN64
#elif !defined(SAG_COM) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#	if defined(WINCE) || defined(_WIN32_WCE)
#   	define CWT_OS_WINCE
#	else
#		define CWT_OS_WIN32
#	endif
#elif defined(__MWERKS__) && defined(__INTEL__)
#	define CWT_OS_WIN32
#elif defined(__sun) || defined(sun)
#	define CWT_OS_SOLARIS
#elif defined(hpux) || defined(__hpux)
#	define CWT_OS_HPUX
#elif defined(__ultrix) || defined(ultrix)
#	define CWT_OS_ULTRIX
#elif defined(sinix)
#	define CWT_OS_RELIANT
#elif defined(__native_client__)
#	define CWT_OS_NACL
#elif defined(__linux__) || defined(__linux)
#	define CWT_OS_LINUX
#elif defined(__FreeBSD__) || defined(__DragonFly__)
#	define CWT_OS_FREEBSD
#	define CWT_OS_BSD4
#elif defined(__NetBSD__)
#	define CWT_OS_NETBSD
#	define CWT_OS_BSD4
#elif defined(__OpenBSD__)
#	define CWT_OS_OPENBSD
#	define CWT_OS_BSD4
#elif defined(__bsdi__)
#	define CWT_OS_BSDI
#	define CWT_OS_BSD4
#elif defined(__sgi)
#	define CWT_OS_IRIX
#elif defined(__osf__)
#	define CWT_OS_OSF
#elif defined(_AIX)
#	define CWT_OS_AIX
#elif defined(__Lynx__)
#	define CWT_OS_LYNX
#elif defined(__GNU__)
#	define CWT_OS_HURD
#elif defined(__DGUX__)
#	define CWT_OS_DGUX
#elif defined(__QNXNTO__)
#	define CWT_OS_QNX
#elif defined(_SEQUENT_)
#	define CWT_OS_DYNIX
#elif defined(_SCO_DS) /* SCO OpenServer 5 + GCC */
#	define CWT_OS_SCO
#elif defined(__USLC__) /* all SCO platforms + UDK or OUDK */
#	define CWT_OS_UNIXWARE
#elif defined(__svr4__) && defined(i386) /* Open UNIX 8 + GCC */
#	define CWT_OS_UNIXWARE
#elif defined(__INTEGRITY)
#	define CWT_OS_INTEGRITY
#elif defined(VXWORKS) /* there is no "real" VxWorks define - this has to be set in the mkspec! */
# 	define CWT_OS_VXWORKS
#elif defined(__MAKEDEPEND__)
#endif

#if defined(CWT_OS_WIN32) || defined(CWT_OS_WIN64) || defined(CWT_OS_WINCE)
#	define CWT_OS_WIN
#endif
#if defined(CWT_OS_DARWIN)
#	define CWT_OS_MAC  /* is mostly for compatibility */
#	define CWT_OS_MACX /* is only for compatibility */
#	if defined(CWT_OS_DARWIN64)
#   	define CWT_OS_MAC64
#	elif defined(CWT_OS_DARWIN32)
#   	define CWT_OS_MAC32
#	endif
#endif

#if defined(CWT_OS_MSDOS) || defined(CWT_OS_OS2) || defined(CWT_OS_WIN)
#	undef CWT_OS_UNIX
#elif !defined(CWT_OS_UNIX)
#	define CWT_OS_UNIX
#endif

#endif /* __CWT_GLOBAL_OS_H__ */

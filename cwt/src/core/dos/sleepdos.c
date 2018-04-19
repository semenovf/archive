/*
 * unistd.c
 *
 *  Created on: 08.07.2012
 *      Author: wladt
 */


#include <cwt/unistd.h>

BOOL cwt_sleep  (ULONG sec)   { sleep(sec); return TRUE; }         /* FIXME check if sleep returns code ?*/
BOOL cwt_msleep (ULONG msec)  { delay(msec); return TRUE; }        /* FIXME check if delay returns code ?*/
BOOL cwt_usleep (ULONG usec)  { delay((usec)/1000); return TRUE; } /* TODO ? */

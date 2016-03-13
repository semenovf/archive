/*
 * assert.c
 *
 *  Created on: Jun 18, 2015
 *      Author: wladt
 */

#include <stdio.h>
#include <pfs.h>

#if defined(__GNUC__)
#	ifndef PFS_BACKTRACE_SIZE
#		define PFS_BACKTRACE_SIZE 20
#	endif
#	ifndef PFS_BACKTRACE_FD
#		define PFS_BACKTRACE_FD    2 /* standard error */
#	endif

#	include <execinfo.h>
#endif


void pfs_assert (const char * file, int line, const char * text)
{
#if defined(__GNUC__)
	/*
	 * Backtrace output
	 */
	fprintf(stderr, "ERROR: (%s[%d]): %s\n", file, line, text);
	void * bt_buffer[PFS_BACKTRACE_SIZE];
	int bt_size = backtrace(bt_buffer, PFS_BACKTRACE_SIZE);
	fprintf(stderr, "===BEGIN OF BACKTRACE\n");
	backtrace_symbols_fd(bt_buffer, bt_size, PFS_BACKTRACE_FD);
	fprintf(stderr, "===END OF BACKTRACE\n");
	abort();
#else
	fprintf(stderr, "ERROR: (%s[%d]): %s\n", file, line, text);
	abort();
#endif
}

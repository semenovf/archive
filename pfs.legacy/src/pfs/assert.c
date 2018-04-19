/*
 * assert.c
 *
 *  Created on: Jun 18, 2015
 *      Author: wladt
 */

#include <stdio.h>
#include <unistd.h>
#include <pfs.h>

#if defined(__GNUC__)
#	ifndef PFS_BACKTRACE_SIZE
#		define PFS_BACKTRACE_SIZE 20
#	endif
#	ifndef PFS_ASSERT_BACKTRACE_FD
#		define PFS_ASSERT_BACKTRACE_FD    2 /* standard error */
#	endif

#	ifndef PFS_BACKTRACE_FD
#		define PFS_BACKTRACE_FD    1 /* standard output */
#	endif

#	include <execinfo.h>
#endif


void pfs_backtrace (const char * file, int line, const char * text)
{
#if defined(__GNUC__)
	/*
	 * Backtrace output
	 */
	fprintf(stdout, "BACKTRACE: (%s[%d]): %s\n", file, line, text ? text : "?");
	void * bt_buffer[PFS_BACKTRACE_SIZE];
	int bt_size = backtrace(bt_buffer, PFS_BACKTRACE_SIZE);
	fprintf(stdout, "===BEGIN OF BACKTRACE\n");
	backtrace_symbols_fd(bt_buffer, bt_size, PFS_BACKTRACE_FD);
	fprintf(stdout, "===END OF BACKTRACE\n");
        fsync(STDOUT_FILENO);
        fsync(STDERR_FILENO);
#else
	fprintf(stdout, "BACKTRACE: (%s[%d]): %s\n", file, line, text ? text : "?");
        fsync(stdout);
#endif
}

void pfs_assert (const char * file, int line, const char * text)
{
#if defined(__GNUC__)
	/*
	 * Backtrace output
	 */
	fprintf(stderr, "ERROR: (%s[%d]): %s\n", file, line, text ? text : "?");
	void * bt_buffer[PFS_BACKTRACE_SIZE];
	int bt_size = backtrace(bt_buffer, PFS_BACKTRACE_SIZE);
	fprintf(stderr, "===BEGIN OF BACKTRACE\n");
	backtrace_symbols_fd(bt_buffer, bt_size, PFS_ASSERT_BACKTRACE_FD);
	fprintf(stderr, "===END OF BACKTRACE\n");
        fsync(STDOUT_FILENO);
        fsync(STDERR_FILENO);
	abort();
#else
	fprintf(stderr, "ERROR: (%s[%d]): %s\n", file, line, text ? text : "?");
        fsync(stderr);
	abort();
#endif
}

void pfs_check_warn (const char * file, int line, const char * text)
{
    fprintf(stderr, "WARN: (%s[%d]): %s\n", file, line, text ? text : "?");
}

void pfs_check_error (const char * file, int line, const char * text)
{
    fprintf(stderr, "ERROR: (%s[%d]): %s\n", file, line, text ? text : "?");
}

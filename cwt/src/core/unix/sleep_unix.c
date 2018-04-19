/**
 * @file   usleep.c
 * @author wladt
 * @date   Nov 29, 2012 4:50:18 PM
 *
 * @brief
 */


#include <cwt/unistd.h>
#include <cwt/logger.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>

#ifdef CWT_MT
#include <pthread.h>

static BOOL cwt_sleep_helper(struct timespec *ts)
{
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;
    int rc;

    /*
        pthread_mutex_init(&lock, 0);
        pthread_cond_init(&cond, 0);
    */

    pthread_mutex_lock(&lock);
   	rc = pthread_cond_timedwait(&cond, &lock, ts);
    pthread_mutex_unlock(&lock);

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&lock);

    return rc == 0 ? TRUE : FALSE;
}
#elif defined(CWT_HAVE_NANOSLEEP)
static BOOL cwt_sleep_helper(struct timespec *ts)
{
	struct timespec rem0, rem1;
	int rc;

	rc = nanosleep(ts, &rem0);
	while (rc == EINTR) {
		rc = nanosleep(&rem0, &rem1);
		rem0.tv_sec  = rem1.tv_sec;
		rem0.tv_nsec = rem1.tv_nsec;
	}
	return rc == 0 ? TRUE : FALSE;
}
#else
#	 error No way to implement sleep family functions
#endif


BOOL cwt_sleep(ULONG secs)
{
    struct timeval tv;
    struct timespec ts;

    gettimeofday(&tv, 0);

    ts.tv_sec = tv.tv_sec + secs;
    ts.tv_nsec = (tv.tv_usec * 1000);
    return cwt_sleep_helper(&ts);
}

BOOL cwt_msleep(ULONG msecs)
{
    struct timeval tv;
    struct timespec ts;

    gettimeofday(&tv, 0);

    ts.tv_nsec = (tv.tv_usec + (msecs % 1000) * 1000) * 1000;
    ts.tv_sec = tv.tv_sec + (msecs / 1000) + (ts.tv_nsec / 1000000000);
    ts.tv_nsec %= 1000000000;

    return cwt_sleep_helper(&ts);
}

BOOL cwt_usleep(unsigned long usecs)
{
    struct timeval tv;
    struct timespec ts;

    gettimeofday(&tv, 0);

    ts.tv_nsec = (tv.tv_usec + (usecs % 1000000)) * 1000;
    ts.tv_sec = tv.tv_sec + (usecs / 1000000) + (ts.tv_nsec / 1000000000);
    ts.tv_nsec %= 1000000000;

    return cwt_sleep_helper(&ts);
}

/*
#if defined(CWT_HAVE_POLL)
#	include <poll.h>
#elif defined(CWT_HAVE_SELECT)
#	include <sys/select.h>
#endif

int cwt_usleep(unsigned int usec)
{
#if defined(CWT_HAVE_POLL)
	return (poll(NULL, 0, usec / 1000));
#elif defined(HAVE_SELECT)
	struct timeval tv;

	tv.tv_sec = 0;
	tv.tv_usec = usec;
	return (select(1, NULL, NULL, NULL, &tv));
#endif
}
*/

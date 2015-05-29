/**
 * @file   posix_signal.cpp
 * @author wladt
 * @date   May 16, 2014
 *
 */

#include <pfs.hpp>
#include <pfs/noncopyable.hpp>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#if defined(_POSIX_SOURCE) || _POSIX_C_SOURCE >= 1 || defined(_XOPEN_SOURCE)
#	include <signal.h>
#else
#	error "Need to support sigaction"
#endif

namespace pfs {

#ifdef __COMMENT__
class posix_signals : public pfs::noncopyable
{
	int max_signum;
	int pipefd[2];

	static void sigaction_handler (int signum) { write(pipefd[1], & signum, sizeof(int)); }

private:
	posix_signals ();

public:
	~posix_signals ();

	void accept(int signum);
	void reset(int signum);
};

posix_signals::posix_signals ()
	: max_signum(0)
{
	pipefd[0] = pipefd[1] = -1;

	if( pipe(pipefd) != 0 ) {
		//sepaloid->addSystemError(errno, "call to 'pipe' failed");
		return;
	}

	fcntl(pipefd[0], F_SETFL, fcntl(pipefd[0], F_GETFL) | O_NONBLOCK);
	fcntl(pipefd[1], F_SETFL, fcntl(pipefd[1], F_GETFL) | O_NONBLOCK);
}

posix_signals::~posix_signals ()
{
	if (pipefd[0] > 0)
		close(pipefd[0]);

	if (pipefd[1] > 0)
		close(pipefd[1]);

	for(int i = 0; i < max_signum; i++ ) {
		signal(i, SIG_DFL);
	}
}

/**
 * Accepts signal by the event subsystem
 *
 * @param signum signal number (see signal(7))
 * @return @b true if specified signal successfully accepted, or @b false otherwise.
 */
bool posix_signals::accept (int signum)
{
	struct sigaction sa;

	sa.sa_handler = posix_signals::sigaction_handler;
	sa.sa_flags = 0;
	sa.sa_restorer = 0;
	sigemptyset(& sa.sa_mask);
	sigaddset(& sa.sa_mask, signum);

	if (sigaction(signum, & sa, NULL) != 0)
		return false;

	max_signum = pfs_max(max_signum, signum);
}

/**
 * Reset signal handler to default behavior
 *
 * @param signum signal number (see signal(7))
 * @return @b true.
 */
void posix_signals::reset(int signum)
{
	signal(signum, SIG_DFL);
}
#endif
} // cwt

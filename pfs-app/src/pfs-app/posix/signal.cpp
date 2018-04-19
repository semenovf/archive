/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <pfs/safeformat.hpp>
#include "pfs/dispatcher.hpp"

namespace pfs {

//static int  __pipefd[2];
//static void __sigaction (int signum) 
//{
//    write(__pipefd[1], & signum, sizeof(int)); 
//}

static dispatcher * __active_dispatcher = 0;

static int const signal_list[] = {
      SIGHUP
    , SIGINT
    , SIGQUIT
    , SIGILL
    , SIGABRT
    , SIGFPE
};

static void __signal_handler (int signum) 
{
    if (__active_dispatcher) {
        switch (signum)  {
        case SIGHUP:
        case SIGINT:
        case SIGQUIT:
        case SIGABRT:
        case SIGILL:
        case SIGFPE:
            __active_dispatcher->emit_quit();
            break;
        }
    }
}


static bool __set_posix_signal_handling (dispatcher * disp
    , void (* handler)(int))
{
    struct sigaction act;
    act.sa_handler = handler;
    act.sa_flags = SA_RESTART;

    // Block every signal during the handler
    sigfillset(& act.sa_mask);
            
    int count = sizeof(signal_list)/sizeof(signal_list[0]);
    
    for (int i = 0; i < count; ++i) {
        if (sigaction(signal_list[i], & act, NULL) < 0) {
            disp->print_error(0, _Sf("sigaction(%d): %s")
                    (signal_list[i])
                    (error_code(errno)).str());
            
            return false;
        }
    }
    
    return true;
}


// activate of handling some posix signals
//
bool dispatcher::activate_posix_signal_handling ()
{
    
//    if (::pipe(__pipefd) != 0) {
//        print_error(0, _Sf("pipe(): %s")(error_code(errno)).str());
//		return false;
//	}
//    
//    ::fcntl(__pipefd[0], F_SETFL, fcntl(__pipefd[0], F_GETFL) | O_NONBLOCK);
//	::fcntl(__pipefd[1], F_SETFL, fcntl(__pipefd[1], F_GETFL) | O_NONBLOCK);
    
    if (! __set_posix_signal_handling(this, & __signal_handler))
        return false;
    
    __active_dispatcher = this;
    
    return true;
}

void dispatcher::deactivate_posix_signal_handling ()
{
    __set_posix_signal_handling(this, SIG_DFL);
    __active_dispatcher = 0;
}

} // pfs
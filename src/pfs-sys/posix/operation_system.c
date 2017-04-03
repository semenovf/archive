#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <pfs/operation_system.h>

#include <stdio.h>

extern pid_t popen3 (char const * command
        , char * const argv[]
        , char * const envp[]
        , int * writefd
        , int * readfd
        , int * errfd);

static void         os_destroy (struct operation_system * self);
static int          os_descover (struct operation_system * self);
static char const * os_type (struct operation_system * self);
static char const * os_family (struct operation_system * self);
static int os_exec (char const * command);
static int os_execv (char const * command
            , char * const argv[]
            , char * const envp[]);
static int os_exec_capture (char const * command
/*            , char const * input, size_t len*/
            , void (* stdout_handler) (char const * buf, size_t n)
            , void (* stderr_handler) (char const * buf, size_t n));
static int os_execv_capture (char const * command
            , char * const argv[]
            , char * const envp[]
/*            , char const * input, size_t len*/
            , void (* stdout_handler) (char const *, size_t)
            , void (* stderr_handler) (char const *, size_t));


typedef struct operation_system_impl
{
    struct operation_system base;
    struct utsname uts;
    /* {
     *      char sysname[];    Operating system name (e.g., "Linux")
     *      char nodename[];   Name within "some implementation-defined network"
     *      char release[];    Operating system release (e.g., "2.6.28")
     *      char version[];    Operating system version
     *      char machine[];    Hardware identifier
     * } 
     */

} operation_system_impl_t;


operation_system_t * operation_system_create ()
{
    operation_system_impl_t * result = malloc(sizeof(operation_system_impl_t)); 
    memset(result, 0, sizeof(operation_system_impl_t));
   
    result->base.destroy       = os_destroy;
    result->base.descover      = os_descover;
    result->base.type          = os_type;
    result->base.family        = os_family;
    result->base.exec          = os_exec;
    result->base.execv         = os_execv;
    result->base.exec_capture  = os_exec_capture;
    result->base.execv_capture = os_execv_capture;

    return (struct operation_system *)result;
}

void os_destroy (operation_system_t * self)
{
    if (self) {
        free(self);
    }
}

int os_descover (struct operation_system * self)
{
    operation_system_impl_t * s = (operation_system_impl_t *)self;
    
    int ex = uname(& s->uts);
    if (ex)
        return errno;
    return 0;
}

char const * os_type (struct operation_system * self)
{
    operation_system_impl_t * s = (operation_system_impl_t *)self;
    
    if (strstr(s->uts.sysname, "linux") != 0
            || strstr(s->uts.sysname, "Linux") != 0
            || strstr(s->uts.sysname, "LINUX") != 0)
        return OS_TYPE_LINUX_S;

    if (strstr(s->uts.sysname, "bsd") != 0
            || strstr(s->uts.sysname, "BSD") != 0)
        return OS_TYPE_BSD_S;
    
    if (strstr(s->uts.sysname, "darwin") != 0
            || strstr(s->uts.sysname, "Darwin") != 0
            || strstr(s->uts.sysname, "DARWIN") != 0)
        return OS_TYPE_MACOS_S;

    if (strstr(s->uts.sysname, "cygwin") != 0
            || strstr(s->uts.sysname, "Cygwin") != 0
            || strstr(s->uts.sysname, "CYGWIN") != 0)
        return OS_TYPE_CYGWIN_S;
    
/*
    if (strstr(s->uts.sysname, "sunos") != 0
            || strstr(s->uts.sysname, "SunOS") != 0
            || strstr(s->uts.sysname, "SUNOS") != 0)
        return OS_TYPE_SOLARIS_S;

    if (strstr(s->uts.sysname, "hp-ux") != 0
            || strstr(s->uts.sysname, "HP-UX") != 0)
        return OS_TYPE_HPUX_S;
*/    
    return OS_TYPE_UNKNOWN_S;
}

char const * os_family (struct operation_system * self)
{
    operation_system_impl_t * s = (operation_system_impl_t *)self;
    char const * type = self->type(self);
    
    if (strcmp(OS_TYPE_LINUX_S, type) == 0
            || strcmp(OS_TYPE_BSD_S, type) == 0
            || strcmp(OS_TYPE_MACOS_S, type) == 0
/*            || strcmp(OS_TYPE_SOLARIS_S, type) == 0
            || strcmp(OS_TYPE_HPUX_S, type) == 0*/)
        return OS_FAMILY_UNIX_S;
    
    if (strcmp(OS_TYPE_MSWIN_S, type) == 0 
            || strcmp(OS_TYPE_CYGWIN_S, type) == 0)
        return OS_FAMILY_MSWIN_S;
    
    if (strcmp(OS_TYPE_DOS_S, type) == 0)
        return OS_FAMILY_DOS_S;
    
    return OS_FAMILY_UNKNOWN_S;
}

//
// [ENV33-C. Do not call system()](https://www.securecoding.cert.org/confluence/pages/viewpage.action?pageId=2130132)
//
int os_exec (char const * command)
{
//    int std_out;
//    int std_err;
//    
//    pid_t result = popen3(command, NULL, & std_out, & std_err);
//
//    if (result < 0) {
//        fprintf(stderr, "errno: %d for command '%s'\n", errno, command);   
//    }
//    
//    return result;
    return -1;
}

int os_execv (char const * command
            , char * const argv[]
            , char * const envp[])
{
    return -1;
}


#define BUFSIZE 256

static int os_exec_capture (char const * command
//            , char const * input, size_t len
            , void (* stdout_handler) (char const *, size_t)
            , void (* stderr_handler) (char const *, size_t))
{
//    int std_in;
//    int std_out;
//    int std_err;
//    
//    pid_t result = popen3(command, & std_in, & std_out, & std_err);
//    
//    if (result > 0) {
//        char buf[BUFSIZE];
//
//        write(std_in, input, len);
//        
//        ssize_t n = 0;
//        
//        while ((n = read(std_out, buf, BUFSIZE)) > 0) {
//            stdout_handler(buf, (size_t)n);
//        }
//
//        while ((n = read(std_err, buf, BUFSIZE)) > 0) {
//            stderr_handler(buf, (size_t)n);
//        }
//    } else {
//        printf("errno: %d for command '%s'\n", errno, command);   
//    }
//        
//    return result;
    return -1;
}

int os_execv_capture (char const * command
            , char * const argv[]
            , char * const envp[]
//            , char const * input, size_t len
            , void (* stdout_handler) (char const *, size_t)
            , void (* stderr_handler) (char const *, size_t))
{
    int std_in;
    int std_out;
    int std_err;
    
    pid_t result = popen3 (command, argv, envp            
            , & std_in, & std_out, & std_err);
    
    if (result > 0) {
        ssize_t n = 0;
        char buf[BUFSIZE];

/*        if (input && len > 0) {
            write(std_in, input, len);
        }
*/        
        if (stdout_handler) {
            while ((n = read(std_out, buf, BUFSIZE)) > 0) {
                stdout_handler(buf, (size_t)n);
            }
        }

        if (stderr_handler) {
            while ((n = read(std_err, buf, BUFSIZE)) > 0) {
                stderr_handler(buf, (size_t)n);
            }
        }
    } else {
        printf("errno: %d for command '%s'\n", errno, command);   
    }

    printf("FINISHED: '%s'\n", command);   
    return result;
}
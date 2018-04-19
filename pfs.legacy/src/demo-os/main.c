#include <stdio.h>
#include <pfs/operation_system.h>

void stdout_handler (char const * buf, size_t n)
{
    write(1, buf, n);
}

void stderr_handler (char const * buf, size_t n)
{
    write(2, "ERROR: ", 7);
    write(2, buf, n);
}

void run_echo (operation_system_t * os, char * msg)
{
    char * argv[] = { "/bin/echo", msg, NULL };
    char * envp[] = { NULL };
    os->execv_capture(argv[0], argv, envp
        , stdout_handler
        , stderr_handler);
}

void run_find (operation_system_t * os, char * dir)
{
    char * argv[] = { "/usr/bin/find", dir, NULL };
    char * envp[] = { NULL };
    os->execv_capture(argv[0], argv, envp
        , stdout_handler
        , stderr_handler);
}

int main (int argc, char * argv[])
{
    operation_system_t * os = operation_system_create();
    os->descover(os);
    
    printf("family = '%s'\n", os->family(os));
    printf("type   = '%s'\n", os->type(os));
    
    run_echo(os, "Hello, World!");
    run_find(os, "/usr/local");
    
    os->destroy(os);
    return 0;
}

#include <stdio.h>
#include <pfs/operation_system.h>

int main (int argc, char * argv[])
{
    operation_system_t * os = operation_system_create();
    os->descover(os);
    
    printf("family = '%s'\n", os->family(os));
    printf("type   = '%s'\n", os->type(os));
    
    os->destroy(os);
    return 0;
}

#include <stdio.h>

int main (int argc, char * argv[])
{
    (void)argc; // avoid 'unused ...' warning
    printf("Hello, %s!\n", argv[0]);
    return 0;
}

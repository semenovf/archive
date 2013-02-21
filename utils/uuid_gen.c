/**
 * @file   utest.c
 * @author see Copyright above
 * @date   Feb 20, 2013 2:08:26 PM
 *
 * @brief
 */


#include <stdio.h>
#include <cwt/uuid.h>

void print_uuid(uuid_t u)
{
    int i;

    printf("%8.8x-%4.4x-%4.4x-%2.2x%2.2x-", u.time_low, u.time_mid,
    u.time_hi_and_version, u.clock_seq_hi_and_reserved,
    u.clock_seq_low);
    for (i = 0; i < 6; i++)
        printf("%2.2x", u.node[i]);
    printf("\n");
}


int main(int argc, char *argv[])
{
    uuid_t u;
    CWT_UNUSED2(argc, argv);
    uuid_create(&u);
    print_uuid(u);
    return 0;
}

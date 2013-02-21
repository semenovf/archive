/**
 * @file   uuid.cpp
 * @author wladt
 * @date   Feb 21, 2013 12:24:57 PM
 *
 * @brief
 */

#include <cwt/test.h>
#include <cstdio>
#include "../include/cwt/uuid.hpp"

using namespace cwt;

const char *uuid_sample_str = "f2cda5e0-7bef-11e2-b92a-0800200c9a66";

uuid_t NameSpace_DNS = { /* 6ba7b810-9dad-11d1-80b4-00c04fd430c8 */
      0x6ba7b810
    , 0x9dad
    , 0x11d1
    , 0x80
    , 0xb4
    , { 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8 }
};

/* puid -- print a UUID */
void puid(uuid_t u)
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
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(5);

    uuid_t u;

    uuid_create(&u);
    printf("uuid_create(): "); puid(u);

    CWT_TEST_OK(uuid_compare(&u, &u) == 0);
    CWT_TEST_OK(uuid_compare(&u, &NameSpace_DNS) > 0);
    CWT_TEST_OK(uuid_compare(&NameSpace_DNS, &u) < 0);

    uuid_create_md5_from_name(&u, NameSpace_DNS, "www.perlinpower.com", 19);
    printf("uuid_create_md5_from_name(): "); puid(u);

    Uuid uuid;
    CWT_TEST_OK(uuid.parse(uuid_sample_str));
    CWT_TEST_OK(String().fromUtf8(uuid_sample_str) == uuid.toString());
    printf("%s\n", uuid.toString().toUtf8().data());

    CWT_END_TESTS;
}



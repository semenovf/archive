/*
** Copyright (c) 1990- 1993, 1996 Open Software Foundation, Inc.
** Copyright (c) 1989 by Hewlett-Packard Company, Palo Alto, Ca. &
** Digital Equipment Corporation, Maynard, Mass.
** Copyright (c) 1998 Microsoft.
** To anyone who acknowledges that this file is provided "AS IS"
** without any express or implied warranty: permission to use, copy,
** modify, and distribute this file for any purpose is hereby
** granted without fee, provided that the above copyright notices and
** this notice appears in all source code copies, and that none of
** the names of Open Software Foundation, Inc., Hewlett-Packard
** Company, Microsoft, or Digital Equipment Corporation be used in
** advertising or publicity pertaining to distribution of the software
** without specific, written prior permission. Neither Open Software
** Foundation, Inc., Hewlett-Packard Company, Microsoft, nor Digital
** Equipment Corporation makes any representations about the
** suitability of this software for any purpose.
*/

/**
 * @file   uuid.c
 * @author see Copyright above
 * @date   Feb 20, 2013 2:01:07 PM
 *
 * @brief
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/cwt/uuid.h"
#include "../include/cwt/md5.h"
#include "../include/cwt/sha1.h"
#include "../include/cwt/mt.h"

#ifdef CWT_CC_GNUC
#	include <unistd.h>
#	include <arpa/inet.h>
#	include <sys/time.h>
#	include <sys/sysinfo.h>
#endif

#ifdef CWT_CC_MSC
#	include <winsock2.h> // htonl, ntohl, etc
#endif


/* set the following to the number of 100ns ticks of the actual
   resolution of your system's clock */
#define UUIDS_PER_TICK 1024

typedef uint64_t uuid_time_t;
typedef struct {
    char nodeID[6];
} uuid_node_t;

static void get_ieee_node_identifier(uuid_node_t *node);
static void get_system_time(uuid_time_t *uuid_time);
static void get_random_info(char seed[16]);

/* various forward declarations */
static int read_state(uint16_t *clockseq, uuid_time_t *timestamp, uuid_node_t *node);
static void write_state(uint16_t clockseq, uuid_time_t timestamp, uuid_node_t node);
static void format_uuid_v1(uuid_t *uuid, uint16_t clockseq, uuid_time_t timestamp, uuid_node_t node);

static void format_uuid_v3or5(uuid_t *uuid, unsigned char hash[16], int v);
static void get_current_time(uuid_time_t *timestamp);
static uint16_t true_random(void);


/* system dependent call to get IEEE node ID.
   This sample implementation generates a random node ID. */
static void get_ieee_node_identifier(uuid_node_t *node)
{
    static int inited = 0;
    static uuid_node_t saved_node;
    char seed[16];
    FILE *fp;

    if (!inited) {
        fp = fopen("nodeid", "rb");
        if (fp) {
            fread(&saved_node, sizeof saved_node, 1, fp);
            fclose(fp);
        }
        else {
            get_random_info(seed);
            seed[0] |= 0x01;
            memcpy(&saved_node, seed, sizeof saved_node);
            fp = fopen("nodeid", "wb");
            if (fp) {
                fwrite(&saved_node, sizeof saved_node, 1, fp);
                fclose(fp);
            }
        }

        inited = 1;
    }

    *node = saved_node;
}

/* system dependent call to get the current system time. Returned as
   100ns ticks since UUID epoch, but resolution may be less than
   100ns. */
#ifdef CWT_CC_MSC

static void get_system_time(uuid_time_t *uuid_time)
{
    ULARGE_INTEGER time;

    /* NT keeps time in FILETIME format which is 100ns ticks since
       Jan 1, 1601. UUIDs use time in 100ns ticks since Oct 15, 1582.
       The difference is 17 Days in Oct + 30 (Nov) + 31 (Dec)
       + 18 years and 5 leap days. */
    GetSystemTimeAsFileTime((FILETIME *)&time);
    time.QuadPart +=
          (unsigned __int64) (1000*1000*10)       // seconds
        * (unsigned __int64) (60 * 60 * 24)       // days
        * (unsigned __int64) (17+30+31+365*18+5); // # of days
    *uuid_time = time.QuadPart;
}

/* Sample code, not for use in production; see RFC 1750 */
static void get_random_info(char seed[16])
{
	md5_state_t ctx; /*MD5_CTX c;*/

    struct {
        MEMORYSTATUS m;
        SYSTEM_INFO s;
        FILETIME t;
        LARGE_INTEGER pc;
        DWORD tc;
        DWORD l;
        char hostname[MAX_COMPUTERNAME_LENGTH + 1];
    } r;

    md5_init(&ctx);/*    MD5Init(&c);*/
    GlobalMemoryStatus(&r.m);
    GetSystemInfo(&r.s);
    GetSystemTimeAsFileTime(&r.t);
    QueryPerformanceCounter(&r.pc);
    r.tc = GetTickCount();
    r.l = MAX_COMPUTERNAME_LENGTH + 1;
    GetComputerNameA(r.hostname, &r.l);
    md5_update(&ctx, (md5_byte_t*)&r, sizeof r); /*   MD5Update(&c, &r, sizeof r);*/
    md5_finish(&ctx, seed); /*    MD5Final(seed, &c);*/
}

#else

static void get_system_time(uuid_time_t *uuid_time)
{
    struct timeval tp;

    gettimeofday(&tp, (struct timezone *)0);

    /* Offset between UUID formatted times and Unix formatted times.
       UUID UTC base time is October 15, 1582.
       Unix base time is January 1, 1970.*/
    *uuid_time = ((uint64_t)tp.tv_sec * 10000000)
        + ((uint64_t)tp.tv_usec * 10)
        + 0x01B21DD213814000LL;
}

/* Sample code, not for use in production; see RFC 1750 */
static void get_random_info(char seed[16])
{
	md5_state_t ctx; /*MD5_CTX c;*/
    struct {
        struct sysinfo s;
        struct timeval t;
        char hostname[257];
    } r;

    md5_init(&ctx);/*    MD5Init(&c);*/
    sysinfo(&r.s);
    gettimeofday(&r.t, (struct timezone *)0);
    gethostname(r.hostname, 256);
    md5_update(&ctx, (md5_byte_t*)&r, sizeof r); /*   MD5Update(&c, &r, sizeof r);*/
    md5_finish(&ctx, (md5_byte_t*)seed); /*    MD5Final(seed, &c);*/
}

#endif

/**
 *
 * @param uuid
 * @return
 */
int uuid_create(uuid_t *uuid)
{
     uuid_time_t timestamp, last_time;
     uint16_t clockseq;
     uuid_node_t node;
     uuid_node_t last_node;
     int f;

     /* acquire system-wide lock so we're alone */
     CWT_LOCK;
     /* get time, node ID, saved state from non-volatile storage */
     get_current_time(&timestamp);
     get_ieee_node_identifier(&node);
     f = read_state(&clockseq, &last_time, &last_node);

     /* if no NV state, or if clock went backwards, or node ID
        changed (e.g., new network card) change clockseq */
     if (!f || memcmp(&node, &last_node, sizeof node))
         clockseq = true_random();
     else if (timestamp < last_time)
         clockseq++;

     /* save the state for next time */
     write_state(clockseq, timestamp, node);

     CWT_UNLOCK;

     /* stuff fields into the UUID */
     format_uuid_v1(uuid, clockseq, timestamp, node);
     return 1;
}

/* format_uuid_v1 -- make a UUID from the timestamp, clockseq,
                     and node ID */
void format_uuid_v1(uuid_t* uuid, uint16_t clock_seq,
                    uuid_time_t timestamp, uuid_node_t node)
{
    /* Construct a version 1 uuid with the information we've gathered
       plus a few constants. */
    uuid->time_low = (unsigned long)(timestamp & 0xFFFFFFFF);
    uuid->time_mid = (unsigned short)((timestamp >> 32) & 0xFFFF);
    uuid->time_hi_and_version =
        (unsigned short)((timestamp >> 48) & 0x0FFF);
    uuid->time_hi_and_version |= (1 << 12);
    uuid->clock_seq_low = clock_seq & 0xFF;
    uuid->clock_seq_hi_and_reserved = (clock_seq & 0x3F00) >> 8;
    uuid->clock_seq_hi_and_reserved |= 0x80;
    memcpy(&uuid->node, &node, sizeof uuid->node);
}

/* data type for UUID generator persistent state */
typedef struct {
    uuid_time_t  ts;       /* saved timestamp */
    uuid_node_t  node;     /* saved node ID */
    uint16_t     cs;       /* saved clock sequence */
} uuid_state;

static uuid_state st;

/* read_state -- read UUID generator state from non-volatile store */
int read_state(uint16_t *clockseq, uuid_time_t *timestamp,
               uuid_node_t *node)
{
    static int inited = 0;
    FILE *fp;

    /* only need to read state once per boot */
    if (!inited) {
        fp = fopen("state", "rb");
        if (fp == NULL)
            return 0;
        fread(&st, sizeof st, 1, fp);
        fclose(fp);
        inited = 1;
    }
    *clockseq = st.cs;
    *timestamp = st.ts;
    *node = st.node;
    return 1;
}

/* write_state -- save UUID generator state back to non-volatile
   storage */
void write_state(uint16_t clockseq, uuid_time_t timestamp,
                 uuid_node_t node)
{
    static int inited = 0;
    static uuid_time_t next_save;
    FILE* fp;

    if (!inited) {
        next_save = timestamp;
        inited = 1;
    }

    /* always save state to volatile shared state */
    st.cs = clockseq;
    st.ts = timestamp;
    st.node = node;
    if (timestamp >= next_save) {
        fp = fopen("state", "wb");
        fwrite(&st, sizeof st, 1, fp);
        fclose(fp);
        /* schedule next save for 10 seconds from now */
        next_save = timestamp + (10 * 10 * 1000 * 1000);
    }
}

/* get-current_time -- get time as 60-bit 100ns ticks since UUID epoch.
   Compensate for the fact that real clock resolution is
   less than 100ns. */
void get_current_time(uuid_time_t *timestamp)
{
    static int inited = 0;
    static uuid_time_t time_last;
    static uint16_t uuids_this_tick;
    uuid_time_t time_now;

    if (!inited) {
        get_system_time(&time_now);
        uuids_this_tick = UUIDS_PER_TICK;
        inited = 1;
    }

    for ( ; ; ) {
        get_system_time(&time_now);

        /* if clock reading changed since last UUID generated, */
        if (time_last != time_now) {
            /* reset count of uuids gen'd with this clock reading */
            uuids_this_tick = 0;
            time_last = time_now;
            break;
        }
        if (uuids_this_tick < UUIDS_PER_TICK) {
            uuids_this_tick++;
            break;
        }
        /* going too fast for our clock; spin */
    }
    /* add the count of uuids to low order bits of the clock reading */
    *timestamp = time_now + uuids_this_tick;
}

/* true_random -- generate a crypto-quality random number.
   **This sample doesn't do that.** */
static uint16_t true_random(void)
{
    static int inited = 0;
    uuid_time_t time_now;

    if (!inited) {
        get_system_time(&time_now);
        time_now = time_now / UUIDS_PER_TICK;
        srand((unsigned int)
               (((time_now >> 32) ^ time_now) & 0xffffffff));
        inited = 1;
    }

    return rand();
}

/**
 * @brief Create a version 3 (MD5) UUID using a "name" from a "name space"
 *
 * @param uuid resulting UUID
 * @param nsid UUID of the namespace
 * @param name the name from which to generate a UUID
 * @param namelen the length of the name
 */
void uuid_create_md5_from_name(uuid_t *uuid, uuid_t nsid, const void *name, int namelen)
{
	md5_state_t ctx; /*MD5_CTX c;*/
    unsigned char hash[16];
    uuid_t net_nsid;

    /* put name space ID in network byte order so it hashes the same
       no matter what endian machine we're on */
    net_nsid = nsid;
    net_nsid.time_low = htonl(net_nsid.time_low);
    net_nsid.time_mid = htons(net_nsid.time_mid);
    net_nsid.time_hi_and_version = htons(net_nsid.time_hi_and_version);

    md5_init(&ctx);/*    MD5Init(&c);*/
    md5_update(&ctx, (md5_byte_t*)&net_nsid, sizeof net_nsid); /*   MD5Update(&c, &net_nsid, sizeof net_nsid); */
    md5_update(&ctx, name, namelen);
    md5_finish(&ctx, hash); /*  MD5Final(hash, &c); */

    /* the hash is in network byte order at this point */
    format_uuid_v3or5(uuid, hash, 3);
}

/**
 * @brief Create a version 5 (SHA-1) UUID using a "name" from a "name space"
 *
 * @param uuid resulting UUID
 * @param nsid UUID of the namespace
 * @param name the name from which to generate a UUID
 * @param namelen the length of the name
 */
void uuid_create_sha1_from_name(uuid_t *uuid, uuid_t nsid, const void *name, int namelen)
{
	sha1_state_t ctx;  /*SHA_CTX c;*/
    unsigned char hash[SHA1_OUTPUT_SIZE];
    uuid_t net_nsid;

    /* put name space ID in network byte order so it hashes the same
       no matter what endian machine we're on */
    net_nsid = nsid;
    net_nsid.time_low = htonl(net_nsid.time_low);
    net_nsid.time_mid = htons(net_nsid.time_mid);
    net_nsid.time_hi_and_version = htons(net_nsid.time_hi_and_version);

    sha1_init(&ctx);
    sha1_update(&ctx, (sha1_byte_t*)&net_nsid, sizeof net_nsid);
    sha1_update(&ctx, name, namelen);
    sha1_finish(&ctx, hash); /*  SHA1_Final(hash, &c); */

    /* the hash is in network byte order at this point */
    format_uuid_v3or5(uuid, hash, 5);
}

/* format_uuid_v3or5 -- make a UUID from a (pseudo)random 128-bit
   number */
void format_uuid_v3or5(uuid_t *uuid, unsigned char hash[16], int v)
{
    /* convert UUID to local byte order */
    memcpy(uuid, hash, sizeof *uuid);
    uuid->time_low = ntohl(uuid->time_low);
    uuid->time_mid = ntohs(uuid->time_mid);
    uuid->time_hi_and_version = ntohs(uuid->time_hi_and_version);

    /* put in the variant and version bits */
    uuid->time_hi_and_version &= 0x0FFF;
    uuid->time_hi_and_version |= (v << 12);
    uuid->clock_seq_hi_and_reserved &= 0x3F;
    uuid->clock_seq_hi_and_reserved |= 0x80;
}

/* uuid_compare --  Compare two UUID's "lexically" and return */
#define CHECK(f1, f2) if (f1 != f2) return f1 < f2 ? -1 : 1;
/* uuid_compare --  Compare two UUID's "lexically" and return
        -1   u1 is lexically before u2
         0   u1 is equal to u2
         1   u1 is lexically after u2
   Note that lexical ordering is not temporal ordering!
*/
int uuid_compare(uuid_t *u1, uuid_t *u2)
{
    int i;

    CHECK(u1->time_low, u2->time_low);
    CHECK(u1->time_mid, u2->time_mid);
    CHECK(u1->time_hi_and_version, u2->time_hi_and_version);
    CHECK(u1->clock_seq_hi_and_reserved, u2->clock_seq_hi_and_reserved);
    CHECK(u1->clock_seq_low, u2->clock_seq_low)
    for (i = 0; i < 6; i++) {
        if (u1->node[i] < u2->node[i])
            return -1;
        if (u1->node[i] > u2->node[i])
            return 1;
    }
    return 0;
}
#undef CHECK

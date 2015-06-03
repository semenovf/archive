/*
 * test.cpp
 *
 *  Created on: Oct 10, 2014
 *      Author: wladt
 */

#include <pfs/test.hpp>
#include <pfs/bits/strtointegral.hpp>
#include <cstring>
#include <iostream>

static const char * sprintf_errno (int ex, char * buffer)
{
    switch(errno) {
    case ERANGE:
        return "ERANGE";
    default:
        break;
    }

    sprintf(buffer, "%d", ex);
    return buffer;
}

inline const char * sprintf_unsigned (uintegral_t n, char * buffer)
{
#if defined(PFS_HAVE_LONGLONG) && (PFS_LLONG_MAX !=  PFS_LONG_MAX)
    sprintf(buffer, "%llu", n);
#else
    sprintf(buffer, "%lu", n);
#endif
    return buffer;
}

inline const char * sprintf_signed (integral_t n, char * buffer)
{
#if defined(PFS_HAVE_LONGLONG) && (PFS_LLONG_MAX !=  PFS_LONG_MAX)
    sprintf(buffer, "%lld", n);
#else
    sprintf(buffer, "%ld", n);
#endif
    return buffer;
}

template <typename T>
bool compare_unsigned (const char * s, uintegral_t num, int errn = 0, int radix = 10)
{
    char buffer[512];
    char num_buf[64];
    char r_buf[64];
    char errno_buf[64];
    char errn_buf[64];
    const char * endptr;
    const char * begin = s;
    const char * end = s + strlen(s);

    uintegral_t r = pfs::strtouintegral<char,const char *>(begin, end, radix
            , static_cast<uintegral_t>(pfs::max_type<T>()), & endptr);


    sprintf(buffer,  "\"%s\": %s == %s AND errno(%s) == %s AND endptr(%p) == %p"
            , s
            , sprintf_unsigned(r, r_buf)
            , sprintf_unsigned(num, num_buf)
            , sprintf_errno(errno, errno_buf)
            , sprintf_errno(errn, errn_buf)
            , endptr, end);
    TEST_OK2(r == num && endptr == end && errno == errn, buffer);

    return r == num && endptr == end && errno == errn;
}

template <typename T>
bool compare_signed (const char * s, integral_t num, int errn = 0, int radix = 10)
{
    char buffer[512];
    char num_buf[64];
    char r_buf[64];
    char errno_buf[64];
    char errn_buf[64];
    const char * endptr;
    const char * begin = s;
    const char * end = s + strlen(s);

    integral_t r = pfs::strtointegral<char, const char *>(begin, end, radix
            , static_cast<integral_t>(pfs::min_type<T>()), static_cast<uintegral_t>(pfs::max_type<T>()), & endptr);

    sprintf(buffer,  "\"%s\": %s == %s AND errno(%s) == %s AND endptr(%p) == %p"
            , s
            , sprintf_unsigned(r, r_buf)
            , sprintf_unsigned(num, num_buf)
            , sprintf_errno(errno, errno_buf)
            , sprintf_errno(errn, errn_buf)
            , endptr, end);
    TEST_OK2(r == num && endptr == end && errno == errn, buffer);

    return r == num && endptr == end && errno == errn;
}

template <typename T>
void test_compare_unsigned_zero_and_one ()
{
    ADD_TESTS(6);
    compare_unsigned<T>("0" , 0, 0);
    compare_unsigned<T>("+0", 0, 0);
    compare_unsigned<T>("-0", 0, 0);
    compare_unsigned<T>("1" , 1, 0);
    compare_unsigned<T>("+1", 1, 0);
    compare_unsigned<T>("-1", static_cast<uintegral_t>(-1), 0);
}

template <typename T>
void test_compare_signed_zero_and_one ()
{
    ADD_TESTS(6);
    compare_signed<T>("0" , 0, 0);
    compare_signed<T>("+0", 0, 0);
    compare_signed<T>("-0", 0, 0);
    compare_signed<T>("1" , 1, 0);
    compare_signed<T>("+1", 1, 0);
    compare_signed<T>("-1", static_cast<uintegral_t>(-1), 0);
}

// U - unsigned type
// S - signed type
template <typename U, typename S>
void test_compare_values ()
{
    ADD_TESTS(43);
    char buf[64];
    uintegral_t umax = pfs::max_type<U>();
    integral_t smax = pfs::max_type<S>();
    integral_t smin = pfs::min_type<S>();

    compare_unsigned<U>(sprintf_unsigned(umax, buf), pfs::max_type<U>(), 0);
    compare_signed<S>(sprintf_signed(smax, buf)    , pfs::max_type<S>(), 0);
    compare_signed<S>(sprintf_signed(smin, buf)    , pfs::min_type<S>(), 0);

    for (int i = 1; i < 11; i++ )
        compare_unsigned<U>(sprintf_unsigned(umax + i, buf), pfs::max_type<uintegral_t>(), ERANGE); // overflow

    for (int i = 0; i < 10; i++ )
        compare_unsigned<U>(sprintf_signed(smin - i, buf), static_cast<uintegral_t>(smin - i), 0);

    for (int i = 1; i < 11; i++ )
        compare_signed<S>(sprintf_signed(smax + 1, buf) , pfs::max_type<integral_t>(), ERANGE); // overflow

    for (int i = 1; i < 11; i++ )
        compare_signed<S>(sprintf_signed(smin - 1, buf), pfs::min_type<integral_t>(), ERANGE); // more underflow
}

template <>
void test_compare_values<uintegral_t, integral_t> ()
{
    ADD_TESTS(25);
    char buf[64];
    uintegral_t umax = pfs::max_type<uintegral_t>();
    integral_t smax = pfs::max_type<integral_t>();
    integral_t smin = pfs::min_type<integral_t>();

    compare_unsigned<uintegral_t>(sprintf_unsigned(umax, buf), pfs::max_type<uintegral_t>(), 0);
    compare_signed<integral_t>(sprintf_signed(smax, buf)    , pfs::max_type<integral_t>(), 0);
    compare_signed<integral_t>(sprintf_signed(smin, buf)    , pfs::min_type<integral_t>(), 0);

    compare_unsigned<uintegral_t>("999999999999999999999999", pfs::max_type<uintegral_t>(), ERANGE); // overflow
    compare_unsigned<uintegral_t>("18446744073709551616", pfs::max_type<uintegral_t>(), ERANGE); // overflow

    for (int i = 0; i < 20; ++i)
        compare_unsigned<uintegral_t>(sprintf_signed(-1 - i, buf), static_cast<uintegral_t>(umax - i), 0);

}

int main(int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);
    BEGIN_TESTS(0);

    test_compare_signed_zero_and_one<signed char>();
    test_compare_unsigned_zero_and_one<unsigned char>();
    test_compare_signed_zero_and_one<signed int>();
    test_compare_unsigned_zero_and_one<unsigned int>();
    test_compare_signed_zero_and_one<signed long>();
    test_compare_unsigned_zero_and_one<unsigned long>();

#ifdef PFS_HAVE_LONGLONG
    test_compare_signed_zero_and_one<signed long long>();
    test_compare_unsigned_zero_and_one<unsigned long long>();
#endif

    if (sizeof(char)  < sizeof(integral_t)) test_compare_values<unsigned char, signed char>();
    if (sizeof(short) < sizeof(integral_t)) test_compare_values<unsigned short, signed short>();
    if (sizeof(int)   < sizeof(integral_t)) test_compare_values<unsigned int, signed int>();
    if (sizeof(long)  < sizeof(integral_t)) test_compare_values<unsigned long, signed long>();
#ifdef PFS_HAVE_LONGLONG
    if (sizeof(long long) < sizeof(integral_t)) test_compare_values<unsigned long long, signed long long>();
#endif

    test_compare_values<uintegral_t, integral_t>();

//    int quant = pfs::max_type<int>()/100;
//    int percent = 0;
//
//    printf("quant = %d; %d\n", quant, 0 % quant);
//
//    for (int i = 0; i <= pfs::max_type<int>(); ++i) {
//        if (i % quant == 0) {
//            printf("%d\r", percent);
//            ++percent;
//        }
//    }

    END_TESTS;
}

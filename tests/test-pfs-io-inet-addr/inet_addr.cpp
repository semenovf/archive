
#include <pfs/test/test.hpp>
#include <pfs/io/inet_addr.hpp>
#include <pfs/string.hpp>
#include <pfs/random.hpp>
#include <iostream>

using pfs::string;
using pfs::io::inet4_addr;
using std::cout;
using std::cerr;
using std::endl;

inline const char * prefix (uint32_t x, int base)
{
    return x == 0 ? ""
           : base == 16
                  ? "0x" : base == 8 ? "0" : "";
}

static string buildInetAddrStr (int addrClass, bool withPort
        , uint32_t a
        , uint32_t b
        , uint32_t c
        , uint32_t d
        , uint32_t p
        , int base)
{
    string r;

    r << prefix(a, base) << string::toString(a, base);

    if (addrClass > 1)
        r << '.' << prefix(b, base) << string::toString(b, base);
    if (addrClass > 2)
        r << '.' << prefix(c, base) << string::toString(c, base);
    if (addrClass > 3)
        r << '.' << prefix(d, base) << string::toString(d, base);
    if (withPort)
        r << ":" << prefix(p, base) << string::toString(p, base);

    return r;
}


bool test_check_valid (int addrClass, bool withPort, int ntests)
{
    bool ok = true;
    pfs::random rnd;

    for (int i = 0; ok && i < ntests; ++i) {
        uint32_t  a, b, c, d, p;

        if (addrClass == 4) {
            a = rnd.rand() % pfs::max_type<uint8_t>();
            b = rnd.rand() % pfs::max_type<uint8_t>();
            c = rnd.rand() % pfs::max_type<uint8_t>();
            d = rnd.rand() % pfs::max_type<uint8_t>();
        } else if (addrClass == 3) {
            a = rnd.rand() % pfs::max_type<uint8_t>();
            b = rnd.rand() % pfs::max_type<uint8_t>();
            c = rnd.rand() % pfs::max_type<uint16_t>();
        } else if (addrClass == 2) {
            a = rnd.rand() % pfs::max_type<uint8_t>();
            b = rnd.rand() % PFS_UINT24_MAX;
        } else if (addrClass == 1) {
            a = rnd.rand();
        } else {
            return false;
        }

        p = rnd.rand() % pfs::max_type<uint16_t>();

        pfs::string addrDecStr = buildInetAddrStr(addrClass, withPort, a, b, c, d, p, 10);
        pfs::string addrOctStr = buildInetAddrStr(addrClass, withPort, a, b, c, d, p, 8);
        pfs::string addrHexStr = buildInetAddrStr(addrClass, withPort, a, b, c, d, p, 16);

        inet4_addr addrDec(addrDecStr);
        inet4_addr addrOct(addrOctStr);
        inet4_addr addrHex(addrHexStr);

//        cout << addrDecStr << ' ' << addrOctStr << ' ' << addrHexStr << endl;

        if (!addrDec.isValid()) {
            cerr << "ERROR: unexpected invalid address in decimal format: " << addrDecStr << endl;
            ok = false;
        }

        if (!addrOct.isValid()) {
            cerr << "ERROR: unexpected invalid address in octal format: " << addrOctStr << endl;
            ok = false;
        }

        if (!addrHex.isValid()) {
            cerr << "ERROR: unexpected invalid address in hexadecimal format: " << addrHexStr << endl;
            ok = false;
        }
    }
    return ok;
}


bool test_check_to_string (const string & format, int ntests)
{
    bool ok = true;
    pfs::random rnd;
    bool withPort = format.endsWith(":p") ? true :false;

    int addrClass = format.startsWith("a.b.c.d")
            ? 4
            : format.startsWith("a.b.c")
                  ? 3
                  : format.startsWith("a.b")
                        ? 2
                        : format.startsWith("a")
                              ? 1 : 0;


    for (int i = 0; ok && i < ntests; ++i) {
        uint32_t  a, b, c, d, p;

        if (addrClass == 4) {
            a = rnd.rand() % pfs::max_type<uint8_t>();
            b = rnd.rand() % pfs::max_type<uint8_t>();
            c = rnd.rand() % pfs::max_type<uint8_t>();
            d = rnd.rand() % pfs::max_type<uint8_t>();
        } else if (addrClass == 3) {
            a = rnd.rand() % pfs::max_type<uint8_t>();
            b = rnd.rand() % pfs::max_type<uint8_t>();
            c = rnd.rand() % pfs::max_type<uint16_t>();
        } else if (addrClass == 2) {
            a = rnd.rand() % pfs::max_type<uint8_t>();
            b = rnd.rand() % PFS_UINT24_MAX;
        } else if (addrClass == 1) {
            a = rnd.rand();
        } else {
            return false;
        }

        p = rnd.rand() % pfs::max_type<uint16_t>();

        pfs::string addrDecStr = buildInetAddrStr(addrClass, withPort, a, b, c, d, p, 10);
        pfs::string addrOctStr = buildInetAddrStr(addrClass, withPort, a, b, c, d, p, 8);
        pfs::string addrHexStr = buildInetAddrStr(addrClass, withPort, a, b, c, d, p, 16);

        inet4_addr addrDec(addrDecStr);
        inet4_addr addrOct(addrOctStr);
        inet4_addr addrHex(addrHexStr);

        string addrDecStr1 = addrDec.toString(format, 10);
        string addrOctStr1 = addrOct.toString(format, 8);
        string addrHexStr1 = addrHex.toString(format, 16);

//        cout << addrDecStr << ' ' << addrOctStr << ' ' << addrHexStr << endl;
//        cout << addrDec.addrData() << endl;
//        cout << addrOct.addrData() << endl;
//        cout << addrHex.addrData() << endl;

        if (addrDecStr != addrDecStr1) {
            cout << "Port: " << addrDec.port() << endl;
            cerr << "ERROR: failed comparison in decimal format: " << addrDecStr << " != " << addrDecStr1 << endl;
            return false;
        }

        if (addrOctStr != addrOctStr1) {
            cerr << "ERROR: failed comparison in octal format: " << addrOctStr << " != " << addrOctStr1 << endl;
            return false;
        }

        if (addrHexStr != addrHexStr1) {
            cerr << "ERROR: failed comparison in hexadecimal format: " << addrHexStr << " != " << addrHexStr1 << endl;
            return false;
        }
    }

    return ok;
}

int main(int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);
    BEGIN_TESTS(17);

    static const int CHECK_VALID_NTESTS = 10000;
    static const int CHECK_STRING_NTESTS = 10000;
    static const char * addrClassesStr[] = { "", "a", "a.b", "a.b.c", "a.b.c.d" };
    string msg1, msg2;

    for (int i = 1; i < 5; ++i) {
        msg1.clear();
        msg2.clear();

        msg1 << string::toString(CHECK_VALID_NTESTS) << " random IP addresses (" << addrClassesStr[i] << " format) w/ port specified are valid";
        msg2 << string::toString(CHECK_VALID_NTESTS) << " random IP addresses (" << addrClassesStr[i] << " format) w/o port specified are valid";

        TEST_OK2(test_check_valid(i, true, CHECK_VALID_NTESTS), msg1.c_str());
        TEST_OK2(test_check_valid(i, false, CHECK_VALID_NTESTS), msg2.c_str());
    }

    TEST_OK(test_check_to_string(_l1("a.b.c.d")  , CHECK_STRING_NTESTS));
    TEST_OK(test_check_to_string(_l1("a.b.c.d")  , CHECK_STRING_NTESTS));
    TEST_OK(test_check_to_string(_l1("a.b.c")    , CHECK_STRING_NTESTS));
    TEST_OK(test_check_to_string(_l1("a.b")      , CHECK_STRING_NTESTS));
    TEST_OK(test_check_to_string(_l1("a")        , CHECK_STRING_NTESTS));
    TEST_OK(test_check_to_string(_l1("a.b.c.d:p"), CHECK_STRING_NTESTS));
    TEST_OK(test_check_to_string(_l1("a.b.c:p")  , CHECK_STRING_NTESTS));
    TEST_OK(test_check_to_string(_l1("a.b:p")    , CHECK_STRING_NTESTS));
    TEST_OK(test_check_to_string(_l1("a:p")      , CHECK_STRING_NTESTS));

    END_TESTS;
}

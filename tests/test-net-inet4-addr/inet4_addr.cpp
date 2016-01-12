
#include <pfs/test/test.hpp>
#include <pfs/string.hpp>
#include <pfs/random.hpp>
#include <pfs/net/inet4_addr.hpp>

#include <iostream>

using pfs::string;
using pfs::net::inet4_addr;
using std::cout;
using std::cerr;
using std::endl;

#define UINT24_MAX 0x00FFFFFF

static const char * prefix (uint32_t x, int base)
{
    return x == 0 ? ""
           : base == 16
                  ? "0x" : base == 8 ? "0" : "";
}

static string build_inet_addr_str (int addrClass
        , uint32_t a
        , uint32_t b
        , uint32_t c
        , uint32_t d
        , int base)
{
	char buffer[64];

    if (addrClass == 1) {
    	switch (base) {
    	case 8:
    		sprintf(buffer, "0%o", a);
    		break;
    	case 10:
    		sprintf(buffer, "%u", a);
    		break;
    	case 16:
    		sprintf(buffer, "0x%X", a);
    		break;
    	}
    } else if (addrClass == 2) {
    	switch (base) {
    	case 8:
    		sprintf(buffer, "0%03o.0%o", a, b);
    		break;
    	case 10:
    		sprintf(buffer, "%u.%u", a, b);
    		break;
    	case 16:
    		sprintf(buffer, "0x%02X.0x%X", a, b);
    		break;
    	}
    } else if (addrClass == 3) {
    	switch (base) {
    	case 8:
    		sprintf(buffer, "0%03o.0%03o.0%o", a, b, c);
    		break;
    	case 10:
    		sprintf(buffer,"%u.%u.%u", a, b, c);
    		break;
    	case 16:
    		sprintf(buffer, "0x%02X.0x%02X.0x%X", a, b, c);
    		break;
    	}
    } else if (addrClass == 4) {
    	switch (base) {
    	case 8:
    		sprintf(buffer, "0%03o.0%03o.0%03o.0%03o", a, b, c, d);
    		break;
    	case 10:
    		sprintf(buffer, "%u.%u.%u.%u", a, b, c, d);
    		break;
    	case 16:
    		sprintf(buffer, "0x%02X.0x%02X.0x%02X.0x%02X", a, b, c, d);
    		break;
    	}
    }

    return string(buffer);
}


bool test_check_valid (int addrClass, int ntests)
{
    bool ok = true;
    pfs::random rnd;

    for (int i = 0; ok && i < ntests; ++i) {
        uint32_t  a, b, c, d;

        if (addrClass == 4) {
            a = rnd.rand() % pfs::max_value<uint8_t>();
            b = rnd.rand() % pfs::max_value<uint8_t>();
            c = rnd.rand() % pfs::max_value<uint8_t>();
            d = rnd.rand() % pfs::max_value<uint8_t>();
        } else if (addrClass == 3) {
            a = rnd.rand() % pfs::max_value<uint8_t>();
            b = rnd.rand() % pfs::max_value<uint8_t>();
            c = rnd.rand() % pfs::max_value<uint16_t>();
        } else if (addrClass == 2) {
            a = rnd.rand() % pfs::max_value<uint8_t>();
            b = rnd.rand() % UINT24_MAX;
        } else if (addrClass == 1) {
            a = rnd.rand();
        } else {
            return false;
        }

        pfs::string addrDecStr = build_inet_addr_str(addrClass, a, b, c, d, 10);
        pfs::string addrOctStr = build_inet_addr_str(addrClass, a, b, c, d, 8);
        pfs::string addrHexStr = build_inet_addr_str(addrClass, a, b, c, d, 16);

        inet4_addr addrDec(addrDecStr);
        inet4_addr addrOct(addrOctStr);
        inet4_addr addrHex(addrHexStr);

//        cout << addrDecStr << ' ' << addrOctStr << ' ' << addrHexStr << endl;

        if (!addrDec) {
            cerr << "ERROR: unexpected invalid address in decimal format: " << addrDecStr << endl;
            ok = false;
        }

        if (!addrOct) {
            cerr << "ERROR: unexpected invalid address in octal format: " << addrOctStr << endl;
            ok = false;
        }

        if (!addrHex) {
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

    int addrClass = format.starts_with(_u8("%a.%b.%c.%d"))
            ? 4
            : format.starts_with(_u8("%a.%b.%C"))
                  ? 3
                  : format.starts_with(_u8("%a.%B"))
                        ? 2
                        : format.starts_with(_u8("%A"))
                              ? 1 : 0;


    for (int i = 0; ok && i < ntests; ++i) {
        uint32_t  a, b, c, d, p;

        if (addrClass == 4) {
            a = rnd.rand() % pfs::max_value<uint8_t>();
            b = rnd.rand() % pfs::max_value<uint8_t>();
            c = rnd.rand() % pfs::max_value<uint8_t>();
            d = rnd.rand() % pfs::max_value<uint8_t>();
        } else if (addrClass == 3) {
            a = rnd.rand() % pfs::max_value<uint8_t>();
            b = rnd.rand() % pfs::max_value<uint8_t>();
            c = rnd.rand() % pfs::max_value<uint16_t>();
        } else if (addrClass == 2) {
            a = rnd.rand() % pfs::max_value<uint8_t>();
            b = rnd.rand() % UINT24_MAX;
        } else if (addrClass == 1) {
            a = rnd.rand();
        } else {
            return false;
        }

        pfs::string addrDecStr = build_inet_addr_str(addrClass, a, b, c, d, 10);
        pfs::string addrOctStr = build_inet_addr_str(addrClass, a, b, c, d, 8);
        pfs::string addrHexStr = build_inet_addr_str(addrClass, a, b, c, d, 16);

        inet4_addr addrDec(addrDecStr);
        inet4_addr addrOct(addrOctStr);
        inet4_addr addrHex(addrHexStr);

        string addrDecStr1 = pfs::to_string(addrDec, format, 10);
        string addrOctStr1 = pfs::to_string(addrOct, format, 8);
        string addrHexStr1 = pfs::to_string(addrHex, format, 16);

//        cout << addrDecStr << ' ' << addrOctStr << ' ' << addrHexStr << endl;
//        cout << addrDec.addrData() << endl;
//        cout << addrOct.addrData() << endl;
//        cout << addrHex.addrData() << endl;

        if (addrDecStr != addrDecStr1) {
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
    BEGIN_TESTS(8);

    static const int CHECK_VALID_NTESTS  = 10000;
    static const int CHECK_STRING_NTESTS = 10000;
    static const char * addrClassesStr[] = { "", "%A", "%a.%B", "%a.%b.%C", "%a.%b.%c.%d" };
    string msg;

    for (int i = 1; i < 5; ++i) {
        msg.clear();

        msg.append(pfs::to_string(CHECK_VALID_NTESTS));
        msg.append(" random IP addresses (");
        msg.append(addrClassesStr[i]);
        msg.append(" format)");

        TEST_OK2(test_check_valid(i, CHECK_VALID_NTESTS), msg.c_str());
    }

    TEST_OK(test_check_to_string(_u8("%a.%b.%c.%d") , CHECK_STRING_NTESTS));
    TEST_OK(test_check_to_string(_u8("%a.%b.%C")    , CHECK_STRING_NTESTS));
    TEST_OK(test_check_to_string(_u8("%a.%B")       , CHECK_STRING_NTESTS));
    TEST_OK(test_check_to_string(_u8("%A")          , CHECK_STRING_NTESTS));

    END_TESTS;
}

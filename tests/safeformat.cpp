/*
 * This tests based on loki::SafeFormat tests
 */

#include <cwt/test.hpp>
#include <pfs/safeformat.hpp>
#include <pfs/string.hpp>
#include <iostream>
#include <utility>


//#include "../SmallObj/timer.h"
//
//#if defined(_MSC_VER)
//#if _MSC_VER >= 1400
//#define sprintf     sprintf_s
//#define _snprintf _snprintf_s
//#else
//#define sprintf     sprintf
//#define _snprintf _snprintf
//#endif
//#endif

using namespace std;

template <class Integral1, class Integral2>
Integral2 randomInt (Integral1 low, Integral2 up)
{
    // From ``Accelerated C++'', page 135:
    // random integer in the range [0, n)
    // We adjust to generate in the range [0, n]
    const Integral2
    low2 = low,
           n = up - low;

    assert(n > 0);

    const size_t bucket_size = RAND_MAX / n;
    PFS_ASSERT(bucket_size > 0);

    Integral2 r;
    do
        r = Integral2(rand() / bucket_size);
    while (r > n);

    r = r + low2;
    PFS_ASSERT(r >= low2 && r <= up);
    return r;
}

pfs::string randomString (unsigned int maxSize)
{
    size_t i = 0;
    size_t n = randomInt(0, maxSize);
    pfs::string result;

    for (; i < n; ++i) {
        result << pfs::ucchar(randomInt('a', 'z'));
    }
    return result;
}

template <class T>
void testCase (const pfs::string & fmt, T value)
{
    char buf[4096];
    pfs::string s = pfs::safeformat(fmt)(value)();

#ifdef _MSC_VER
    const int i2 =_snprintf(buf, sizeof(buf), fmt.c_str(), value);
#else
    const int i2 = snprintf(buf, sizeof(buf), fmt.c_str(), value);
#endif

    if (s.length() != i2 || s != pfs::string(buf)) {
        cout << endl
        << "Reference: " << i2 << "; Actual: " << s.length() << ", Difference = " << i2 - s.length() << endl
        << "V: [" << value << "]" << endl
        << "F: [" << fmt << "]" << endl
        << "R: [" << buf << "]" << endl
        << "A: [" << s.c_str() << "]" << endl;
        assert(false);
    }
}

/*
template <class T, class U>
void TestCase2(const string& fmt, T value, U value2)
{
    char buf[4096];
    std::string s;
    const int i1 = SPrintf(s, fmt.c_str())(value)(value2);
    const int i2 = snprintf(buf, sizeof(buf), fmt.c_str(), value, value2);
    assert(i1 == i2);
    assert(s == buf);
}
*/

int main(int argc, char** argv)
{        
	PFS_CHECK_SIZEOF_TYPES;
	PFS_UNUSED2(argc, argv);
	int ntests = 24;
	BEGIN_TESTS(ntests);

#ifdef __COMMENT__
    if (argc == 2) {
        // test speed

        Timer t;

        int loop = atoi(argv[1]);

        if(loop < 100)
            loop = 100;

        t.start();
        for (int i=loop; i > 0; --i)
            printf("Hey, %u frobnicators and %u twiddlicators\n",i, i);
        t.stop();
        t.t100 = t.t();
        int t_printf = t.t();


        t.start();
        for (int i=loop; i > 0; --i)
            Printf("Hey, %u frobnicators and %u twiddlicators\n")(i)(i);
        t.stop();
        int t_Printf = t.t();


        t.start();
        for (int i=loop; i > 0; --i)
            cout << "Hey, " << i << " frobnicators and " << i <<" twiddlicators\n";
        t.stop();
        int t_cout = t.t();


        Printf("\n\nElapsed time for %i outputs\n\n")(loop);
        t.print(t_printf,"printf   : ");
        t.print(t_Printf,"Printf   : ");
        t.print(t_cout,  "std::cout: ");
    }
    else
    {
#endif
        //srand(time(0));
        srand(0);
        for (unsigned i = 0; ; ++i) {
            printf("%u\r", i);

            // Generate a random string for the head
            pfs::string lead = randomString(100);

            // This string will hold a random format specification
            pfs::string formatSpec(lead + "|%");

            // Generate a random set of flags
            static const pfs::string flags("-+0 #");
            size_t maxFlags = randomInt(0u, flags.length() - 1);

            for (size_t i = 0; i != maxFlags; ++i)
            {
                formatSpec += flags[randomInt(0u, flags.length() - 1)];
            }

            // Generate an optional random width
            if (randomInt(0, 1)) {
                const unsigned int width = randomInt(0, 100);
                char buf[4];
                sprintf(buf, "%u", width);
                formatSpec += buf;
            }

            // Generate an optional random precision
            if (randomInt(0, 1)) {
                const unsigned int prec = randomInt(0, 100);
                char buf[4];
                sprintf(buf, "%u", prec);
                formatSpec += '.';
                formatSpec += buf;
            }

            // Generate a random type character
            static const pfs::string type("cdeEfgGiopsuxX");
            pfs::ucchar typeSpec = type.charAt(randomInt(0u, type.length() - 1));

            // Generate an optional type prefix
//            static const pfs::string prefix("hl");
//            if (typeSpec != 's' && randomInt(0, 1)) {
//                formatSpec += prefix[randomInt(0u, prefix.size() - 1)];
//            }
            formatSpec += typeSpec;
            formatSpec += '|';
            formatSpec += randomString(100);

            switch (typeSpec.value()) {
            case 'c':
                testCase(formatSpec, randomInt(1, 127));
                break;
            case 'd':
            case 'i':
            case 'o':
            case 'u':
            case 'x':
            case 'X':
                //TestCase(formatSpec, RandomInt(-10000, 10000));
                // don't test negative values on 64bit systems, because 
                // snprintf does not support 64 Bit values
                testCase(formatSpec, randomInt( -10000 * (sizeof(size_t)>4 ? 0 : 1) , 10000));
                break;
            case 'e':
            case 'E':
            case 'f':
            case 'g':
            case 'G':
                testCase(formatSpec,
                         randomInt(-10000, 10000) / double(randomInt(1, 100)));
                break;
            case 'n':
                break;
            case 'p':
                {
                    void * p = malloc(randomInt(1, 1000));
                    testCase(formatSpec, p);
                    free(p);
                }
                break;
            case 's':
                testCase(formatSpec, randomString(100).c_str());
                break;
            default:
                PFS_ASSERT(false);
                break;
            }
        }
#ifdef __COMMENT__
    }
#endif
}

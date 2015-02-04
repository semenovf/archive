/*
 * This tests based on loki::SafeFormat tests
 */

#include <cwt/test.hpp>
#include <pfs/safeformat.hpp>
#include <pfs/string.hpp>
#include <cwt/stopwatch.hpp>
#include <iostream>
#include <sstream>
#include <utility>

#ifdef PFS_HAVE_QT
#include <QString>
#endif

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
bool testCase (const pfs::string & fmt, T value)
{
    char buf[4096];
    pfs::string s = pfs::safeformat(fmt)(value)();

#ifdef PFS_CC_MSC
    int i2 =_snprintf(buf, sizeof(buf), fmt.c_str(), value);
#else
    int i2 = snprintf(buf, sizeof(buf), fmt.c_str(), value);
#endif

    if (s.length() != i2 || s != pfs::string(buf)) {
    	cout<< endl
        << "Reference: " << i2 << "; Actual: " << s.length() << ", Difference = " << i2 - int(s.length()) << endl
        << "V: [" << value << "]" << endl
        << "F: [" << fmt << "]" << endl
        << "R: [" << buf << "]" << endl
        << "A: [" << s.c_str() << "]" << endl;
        return false;
    }
    return true;
}

void test0 ()
{
	TEST_OK(pfs::safeformat("%d")(2)() == "2");
	TEST_OK(pfs::safeformat("%.5f")(3.14159f)() == "3.14159");
	TEST_OK(pfs::safeformat("eqfhxeuvkmybvuuwwxpathgycwbxewqiovqmbljlyinueisbhicbqjbshcrlaiuofmcg|%+0 s|pmbwvdynnahudahdjcsopvuohomcugbktdhqggeuhmplmwpvaikpegemuroqxqbstjkbqpvxcmjojylkiwandfaywppvgrpbb")("HELLO")()
				== "eqfhxeuvkmybvuuwwxpathgycwbxewqiovqmbljlyinueisbhicbqjbshcrlaiuofmcg|HELLO|pmbwvdynnahudahdjcsopvuohomcugbktdhqggeuhmplmwpvaikpegemuroqxqbstjkbqpvxcmjojylkiwandfaywppvgrpbb");
}

void test1 ()
{
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

        for (size_t i = 0; i != maxFlags; ++i) {
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
        	TEST_FAIL2(testCase(formatSpec, randomInt(1, 127))
        			, formatSpec.c_str());
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
            TEST_FAIL2(testCase(formatSpec, randomInt( -10000 * (sizeof(size_t)>4 ? 0 : 1) , 10000))
            		, formatSpec.c_str());
            break;
        case 'e':
        case 'E':
        case 'f':
        case 'g':
        case 'G':
        	TEST_FAIL2(testCase(formatSpec,randomInt(-10000, 10000) / double(randomInt(1, 100)))
        			, formatSpec.c_str());
            break;
        case 'n':
            break;
        case 'p':
            {
                void * p = malloc(randomInt(1, 1000));
                TEST_FAIL2(testCase(formatSpec, p)
                		, formatSpec.c_str());
                free(p);
            }
            break;
        case 's':
        	TEST_FAIL2(testCase(formatSpec, randomString(100).c_str())
        			, formatSpec.c_str());
            break;
        default:
            PFS_ASSERT(false);
            break;
        }
    }
}

// test speed
void test2 ()
{
	char buf[512];
	using cwt::test::stopwatch;
	stopwatch sw;

	int loop = 100;

	double ellapsed_sprintf;
	double ellapsed_safeformat;
	double ellapsed_sstream;
#ifdef PFS_HAVE_QT
	double ellapsed_qstring;
#endif
//	if(loop < 100)
//		loop = 100;

	sw.start();
	for (int i = loop; i > 0; --i)
		sprintf(buf, "Hey, %u frobnicators and %u twiddlicators\n", i, i);
	ellapsed_sprintf = sw.ellapsed();

	sw.start();
	for (int i = loop; i > 0; --i)
		pfs::safeformat("Hey, %u frobnicators and %u twiddlicators\n")(i)(i);
	ellapsed_safeformat = sw.ellapsed();

	sw.start();
	for (int i = loop; i > 0; --i)
		std::stringstream() << "Hey, " << i << " frobnicators and " << i <<" twiddlicators\n";
	ellapsed_sstream = sw.ellapsed();

#ifdef PFS_HAVE_QT
	sw.start();
	for (int i = loop; i > 0; --i)
		QString("Hey, %1 frobnicators and %2 twiddlicators\n").arg(i).arg(i);
	ellapsed_qstring = sw.ellapsed();
#endif

	cout << endl << "Elapsed time for " << loop << " outputs:" << endl
	     << "\tprintf       = " << ellapsed_sprintf    << endl
	     << "\tsafeformat   = " << ellapsed_safeformat << endl
		 << "\tstringstream = " << ellapsed_sstream    << endl
#ifdef PFS_HAVE_QT
		 << "\tQString      = " << ellapsed_qstring    << endl
#endif
		 ;
}

int main(int argc, char** argv)
{
	PFS_CHECK_SIZEOF_TYPES;
	PFS_UNUSED2(argc, argv);
	int ntests = 24;
	BEGIN_TESTS(ntests);

	if (argc > 1) {
		if (1) test2();
	} else {
		if (1) test0();
		if (1) test1();
	}

	END_TESTS;
}

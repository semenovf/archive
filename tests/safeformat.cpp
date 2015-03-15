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
#include <typeinfo>

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

    PFS_ASSERT(n > 0);

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
//	TEST_FAIL(pfs::safeformat("%d")(2)() == "2");
//	TEST_FAIL(pfs::safeformat("%.5f")(3.14159f)() == "3.14159");
//	TEST_FAIL(pfs::safeformat("eqfhxeuvkmybvuuwwxpathgycwbxewqiovqmbljlyinueisbhicbqjbshcrlaiuofmcg|%+0 s|pmbwvdynnahudahdjcsopvuohomcugbktdhqggeuhmplmwpvaikpegemuroqxqbstjkbqpvxcmjojylkiwandfaywppvgrpbb")("HELLO")()
//				== "eqfhxeuvkmybvuuwwxpathgycwbxewqiovqmbljlyinueisbhicbqjbshcrlaiuofmcg|HELLO|pmbwvdynnahudahdjcsopvuohomcugbktdhqggeuhmplmwpvaikpegemuroqxqbstjkbqpvxcmjojylkiwandfaywppvgrpbb");
}

#ifdef __COMMENT__
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
#endif

#ifdef __COMMENT__
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
#endif


#ifdef __COMMENT__
template <typename T>
bool test_arg(const char * format, T v)
{
	char buf[64];
	sprintf(buf, format, v);
	printf("sprintf: \"%s\" == \"%s\" : SafeFormat\n", buf, pfs::string(pfs::safeformat(format) % v).c_str());
	return pfs::safeformat(format) % v == pfs::string(buf);
}


int main(int argc, char *argv[])
{
	PFS_CHECK_SIZEOF_TYPES;
	PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(38);

	// When 0 is printed with an explicit precision 0, the output is empty.
	TEST_OK(pfs::safeformat("%.0d") % 0 != pfs::string("0"));
	TEST_OK(test_arg<int>("%.0d", 0));

	TEST_OK(test_arg<int>("% d", -245));
	TEST_OK(test_arg<int>("% d",  245));
	TEST_OK(test_arg<int>("%+d", -245));
	TEST_OK(test_arg<int>("%+d",  245));

	TEST_OK(test_arg<int>("%.6d" , 245));
	TEST_OK(test_arg<int>("%06d" , 245));
	TEST_OK(test_arg<int>("%6d"  , 245));
	TEST_OK(test_arg<int>("%6.6d", 245));
	TEST_OK(test_arg<int>("%7.6d", 245));
	TEST_OK(test_arg<int>("%6.5d", 245));
	TEST_OK(test_arg<int>("%7.6o", 245));
	TEST_OK(test_arg<int>("%7.6X", 245));

	TEST_OK(test_arg<int>("%.6d" , 2147483647));
	TEST_OK(test_arg<int>("%06d" , 2147483647));
	TEST_OK(test_arg<int>("%6d"  , 2147483647));
	TEST_OK(test_arg<int>("%6.6d", 2147483647));
	TEST_OK(test_arg<int>("%7.6d", 2147483647));
	TEST_OK(test_arg<int>("%6.5d", 2147483647));

	TEST_OK(test_arg<float>("%.6f"   , 245.9876542f));
	TEST_OK(test_arg<float>("%06f"   , 245.9876542f));
	TEST_OK(test_arg<float>("%6f"    , 245.9876542f));
	TEST_OK(test_arg<float>("%6.6f"  , 245.9876542f));
	TEST_OK(test_arg<float>("%7.6f"  , 245.9876542f));
	TEST_OK(test_arg<float>("%6.5f"  , 245.9876542f));
	TEST_OK(test_arg<float>("%-20f"  , 245.9876542f));
	TEST_OK(test_arg<float>("%-20.5f", 245.98f     ));
	TEST_OK(test_arg<float>("%.6f"   , 245.98f     ));
	TEST_OK(test_arg<float>("%06f"   , 245.98f     ));
	TEST_OK(test_arg<float>("%6f"    , 245.98f     ));
	TEST_OK(test_arg<float>("%6.6f"  , 245.98f     ));
	TEST_OK(test_arg<float>("%7.6f"  , 245.98f     ));
	TEST_OK(test_arg<float>("%6.5f"  , 245.98f     ));

	TEST_OK(test_arg<char>("%05c" , 'A')); // gcc warning: '0' flag used with ‘%c’ gnu_printf format
	TEST_OK(test_arg<char>("%-5c" , 'A'));
	TEST_OK(test_arg<char>("%+5c" , 'A')); // gcc warning: '+' flag used with ‘%c’ gnu_printf format

	TEST_OK(test_arg<int>("%d\n", 245));

	int i;
	pfs::string s = pfs::safeformat("Hello, %s:\t iptr = %p! 0x%X 0x%x") % "World" % &i % 255 % 255;
	printf("%s\n", s.c_str());

	END_TESTS;
}
#endif


template <typename T>
void testCaseIntegral()
{
	cout << "\n\nTesting with [T = " << typeid(T).name() << "]\n";

	TEST_FAIL(testCase<T>(_l1("%o") , pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+o"), pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%o") , pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+o"), pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%o") , 0));
	TEST_FAIL(testCase<T>(_l1("%+o"), 0));

	TEST_FAIL(testCase<T>(_l1("%x") , pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+x"), pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%x") , pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+x"), pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%x") , 0));
	TEST_FAIL(testCase<T>(_l1("%+x"), 0));

	TEST_FAIL(testCase<T>(_l1("%d") , pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+d"), pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%d") , pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+d"), pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%d") , 0));
	TEST_FAIL(testCase<T>(_l1("%+d"), 0));

	TEST_FAIL(testCase<T>(_l1("%u") , pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+u"), pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%u") , pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+u"), pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%u") , 0));
	TEST_FAIL(testCase<T>(_l1("%+u"), 0));

	TEST_FAIL(testCase<T>(_l1("%c"), T('W')));
}

void test3 ()
{
//	TEST_FAIL(testCase<long>(_l1("%u") , -1L));
//
//	return;
	testCaseIntegral<char>();
	testCaseIntegral<signed char>();
	testCaseIntegral<unsigned char>();
	testCaseIntegral<short>();
	testCaseIntegral<unsigned short>();
	testCaseIntegral<int>();
	testCaseIntegral<unsigned int>();
	testCaseIntegral<long>();
	testCaseIntegral<unsigned long>();

#ifdef PFS_HAVE_LONGLONG
	testCaseIntegral<long long>();
	testCaseIntegral<unsigned long long>();
#endif

//	TEST_FAIL(testCase<double>(_l1("mrkh|%  +g|pwvnlnsfcyxumgjphjpggs"), PFS_DOUBLE_MIN));
//	TEST_FAIL(testCase<double>(_l1("mrkh|%  +g|pwvnlnsfcyxumgjphjpggs"), PFS_DOUBLE_MAX));
//
//#ifdef PFS_HAVE_LONG_DOUBLE
//	TEST_FAIL(testCase<long double>(_l1("mrkh|%  +g|pwvnlnsfcyxumgjphjpggs"), PFS_REAL_MIN));
//	TEST_FAIL(testCase<long double>(_l1("mrkh|%  +g|pwvnlnsfcyxumgjphjpggs"), PFS_REAL_MAX));
//#endif

}


int main(int argc, char** argv)
{
	PFS_CHECK_SIZEOF_TYPES;
	PFS_UNUSED2(argc, argv);
	int ntests = 24;
#ifdef PFS_HAVE_LONG_DOUBLE
	ntests += 2;
#endif
	BEGIN_TESTS(ntests);

#ifdef PFS_CC_MSC
	pfs::safeformat::setGlobalCompat(pfs::safeformat::CompatMSC);
#else
	pfs::safeformat::setGlobalCompat(pfs::safeformat::CompatGCC);
#endif

	test3();

//	if (argc > 1) {
//		if (1) test2();
//	} else {
//		if (1) test0();
//		if (1) test1();
//	}

	END_TESTS;
}

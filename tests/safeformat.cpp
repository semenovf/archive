/*
 * This tests based on loki::SafeFormat tests
 */

#include <pfs/test.hpp>
#include <pfs/safeformat.hpp>
#include <pfs/string.hpp>
#include <pfs/typeinfo.hpp>
#include <pfs/stopwatch.hpp>
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
bool testCase (const pfs::string & fmt1, const pfs::string & fmt2, T value)
{
    char buf[5020];
    pfs::string s = pfs::safeformat(fmt1)(value)();

#ifdef PFS_CC_MSC
    int i2 =_snprintf(buf, sizeof(buf), fmt2.c_str(), value);
#else
    int i2 = snprintf(buf, sizeof(buf), fmt2.c_str(), value);
#endif

    if (s.length() != static_cast<size_t>(i2) || s != pfs::string(buf)) {
    	cout<< endl
        << "Reference: " << i2 << "; Actual: " << s.length() << ", Difference = " << i2 - int(s.length()) << endl
        << "V: [" << value << "]" << endl
        << "F1: [" << fmt1 << "]" << endl
		<< "F2: [" << fmt2 << "]" << endl
        << "R: [" << buf << "]" << endl
        << "A: [" << s.c_str() << "]" << endl;
        return false;
    }
    return true;
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

// test speed
void test2 ()
{
	char buf[512];
	using pfs::test::stopwatch;
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

template <typename T>
void testCaseIntegral()
{
	cout << "\n\nTesting with [T = " << pfs::type_name<T>() << "]\n";

	TEST_FAIL(testCase<T>(_l1("%o") , _l1("%o") , pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+o"), _l1("%+o"), pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%o") , _l1("%o") , pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+o"), _l1("%+o"), pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%o") , _l1("%o") , 0));
	TEST_FAIL(testCase<T>(_l1("%+o"), _l1("%+o"), 0));

	TEST_FAIL(testCase<T>(_l1("%x") , _l1("%x") , pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+x"), _l1("%+x"), pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%x") , _l1("%x") , pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+x"), _l1("%+x"), pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%x") , _l1("%x") , 0));
	TEST_FAIL(testCase<T>(_l1("%+x"), _l1("%+x"), 0));

	TEST_FAIL(testCase<T>(_l1("%d") , _l1("%d") , pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+d"), _l1("%+d"), pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%d") , _l1("%d") , pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+d"), _l1("%+d"), pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%d") , _l1("%d") , 0));
	TEST_FAIL(testCase<T>(_l1("%+d"), _l1("%+d"), 0));

	TEST_FAIL(testCase<T>(_l1("%u") , _l1("%u") , pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+u"), _l1("%+u"), pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%u") , _l1("%u") , pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+u"), _l1("%+u"), pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%u") , _l1("%u") , 0));
	TEST_FAIL(testCase<T>(_l1("%+u"), _l1("%+u"), 0));

	TEST_FAIL(testCase<T>(_l1("%c") , _l1("%c"), T('W')));
}

template <typename T>
void testCaseLongIntegral()
{
	cout << "\n\nTesting with [T = " << pfs::type_name<T>() << "]\n";

	TEST_FAIL(testCase<T>(_l1("%o") , _l1("%lo") , pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+o"), _l1("%+lo"), pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%o") , _l1("%lo") , pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+o"), _l1("%+lo"), pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%o") , _l1("%lo") , 0));
	TEST_FAIL(testCase<T>(_l1("%+o"), _l1("%+lo"), 0));

	TEST_FAIL(testCase<T>(_l1("%x") , _l1("%lx") , pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+x"), _l1("%+lx"), pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%x") , _l1("%lx") , pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+x"), _l1("%+lx"), pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%x") , _l1("%lx") , 0));
	TEST_FAIL(testCase<T>(_l1("%+x"), _l1("%+lx"), 0));

	TEST_FAIL(testCase<T>(_l1("%d") , _l1("%ld") , pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+d"), _l1("%+ld"), pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%d") , _l1("%ld") , pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+d"), _l1("%+ld"), pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%d") , _l1("%ld") , 0));
	TEST_FAIL(testCase<T>(_l1("%+d"), _l1("%+ld"), 0));

	TEST_FAIL(testCase<T>(_l1("%u") , _l1("%lu") , pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+u"), _l1("%+lu"), pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%u") , _l1("%lu") , pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+u"), _l1("%+lu"), pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%u") , _l1("%lu") , 0));
	TEST_FAIL(testCase<T>(_l1("%+u"), _l1("%+lu"), 0));

	TEST_FAIL(testCase<T>(_l1("%c") , _l1("%lc"), T('W')));
}

#ifdef PFS_HAVE_LONGLONG
template <typename T>
void testCaseLongLongIntegral()
{
	cout << "\n\nTesting with [T = " << pfs::type_name<T>() << "]\n";

	TEST_FAIL(testCase<T>(_l1("%o") , _l1("%llo") , pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+o"), _l1("%+llo"), pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%o") , _l1("%llo") , pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+o"), _l1("%+llo"), pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%o") , _l1("%llo") , 0));
	TEST_FAIL(testCase<T>(_l1("%+o"), _l1("%+llo"), 0));

	TEST_FAIL(testCase<T>(_l1("%x") , _l1("%llx") , pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+x"), _l1("%+llx"), pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%x") , _l1("%llx") , pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+x"), _l1("%+llx"), pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%x") , _l1("%llx") , 0));
	TEST_FAIL(testCase<T>(_l1("%+x"), _l1("%+llx"), 0));

	TEST_FAIL(testCase<T>(_l1("%d") , _l1("%lld") , pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+d"), _l1("%+lld"), pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%d") , _l1("%lld") , pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+d"), _l1("%+lld"), pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%d") , _l1("%lld") , 0));
	TEST_FAIL(testCase<T>(_l1("%+d"), _l1("%+lld"), 0));

	TEST_FAIL(testCase<T>(_l1("%u") , _l1("%llu") , pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+u"), _l1("%+llu"), pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%u") , _l1("%llu") , pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%+u"), _l1("%+llu"), pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%u") , _l1("%llu") , 0));
	TEST_FAIL(testCase<T>(_l1("%+u"), _l1("%+llu"), 0));

	TEST_FAIL(testCase<T>(_l1("%c") , _l1("%llc"), T('W')));
}
#endif

template <typename T>
void testCaseFloat()
{
	cout << "\n\nTesting with [T = " << pfs::type_name<T>() << "]\n";

	TEST_FAIL(testCase<T>(_l1("%e") , _l1("%e") , pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%e") , _l1("%e") , pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%E") , _l1("%E") , pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%E") , _l1("%E") , pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%f") , _l1("%f") , pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%f") , _l1("%f") , pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%g") , _l1("%g") , pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%g") , _l1("%g") , pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%G") , _l1("%G") , pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%G") , _l1("%G") , pfs::max_type<T>()));
}

template <typename T>
void testCaseLongDouble ()
{
	cout << "\n\nTesting with [T = " << pfs::type_name<T>() << "]\n";

	TEST_FAIL(testCase<T>(_l1("%e") , _l1("%Le") , pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%e") , _l1("%Le") , pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%E") , _l1("%LE") , pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%E") , _l1("%LE") , pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%f") , _l1("%Lf") , pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%f") , _l1("%Lf") , pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%g") , _l1("%Lg") , pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%g") , _l1("%Lg") , pfs::max_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%G") , _l1("%LG") , pfs::min_type<T>()));
	TEST_FAIL(testCase<T>(_l1("%G") , _l1("%LG") , pfs::max_type<T>()));
}


void test0 ()
{
	pfs::string s = pfs::safeformat("%f")(pfs::max_type<double>())();
	cout << s << endl;

	testCaseIntegral<char>();
	testCaseIntegral<signed char>();
	testCaseIntegral<unsigned char>();
	testCaseIntegral<short>();
	testCaseIntegral<unsigned short>();
	testCaseIntegral<int>();
	testCaseIntegral<unsigned int>();
	testCaseLongIntegral<long>();
	testCaseLongIntegral<unsigned long>();

#ifdef PFS_HAVE_LONGLONG
	testCaseLongLongIntegral<long long>();
	testCaseLongLongIntegral<unsigned long long>();
#endif

	testCaseFloat<float>();
	testCaseFloat<double>();

#ifdef PFS_HAVE_LONG_DOUBLE
	testCaseLongDouble<long double>();
#endif
}


int main(int argc, char** argv)
{
	PFS_UNUSED2(argc, argv);
	int ntests = 245;
#ifdef PFS_HAVE_LONGLONG
	ntests += 50;
#endif
#ifdef PFS_HAVE_LONG_DOUBLE
	ntests += 10;
#endif
	BEGIN_TESTS(ntests);

#ifdef PFS_CC_MSC
	pfs::safeformat::setGlobalCompat(pfs::safeformat::CompatMSC);
#else
	pfs::safeformat::setGlobalCompat(pfs::safeformat::CompatGCC);
#endif

	test0();
	test2();

//	if (argc > 1) {
//		if (1) test2();
//	} else {
//		if (1) test1();
//	}

	END_TESTS;
}

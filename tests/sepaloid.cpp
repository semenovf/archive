#include <pfs/sepaloid.hpp>
#include <pfs/test/test.hpp>
#include <pfs/logger.hpp>
#include <pfs/safeformat.hpp>
#include <cstring>

class LocalPetaloidEmitter : public pfs::petaloid
{
public:
	LocalPetaloidEmitter() : pfs::petaloid("LocalPetalodEmitter") {}
	virtual ~LocalPetaloidEmitter() {}
	virtual const pfs::emitter_mapping * getEmitters (int * count);
	virtual const pfs::detector_mapping * getDetectors (int * count) { PFS_UNUSED(count); return NULL; }

public: /*signal*/
	pfs::signal0<> emitZeroArg;
	pfs::signal1<bool> emitOneArg;
	pfs::signal2<bool, char> emitTwoArgs;
	pfs::signal3<bool, char, short> emitThreeArgs;
	pfs::signal4<bool, char, short, int> emitFourArgs;
	pfs::signal5<bool, char, short, int, long> emitFiveArgs;
	pfs::signal6<bool, char, short, int, long, const char *> emitSixArgs;
	pfs::signal7<bool, char, short, int, long, const char *, const pfs::string &> emitSevenArgs;
	pfs::signal8<bool, char, short, int, long, long long, const char*, const pfs::string &> emitEightArgs;
};

const pfs::emitter_mapping* LocalPetaloidEmitter::getEmitters(int *count)
{
	static pfs::emitter_mapping local_emitter_mapping[] = {
		  { 0 , EMITTER_CAST(emitZeroArg) }
		, { 1 , EMITTER_CAST(emitOneArg) }
		, { 2 , EMITTER_CAST(emitTwoArgs) }
		, { 3 , EMITTER_CAST(emitThreeArgs) }
		, { 4 , EMITTER_CAST(emitFourArgs) }
		, { 5 , EMITTER_CAST(emitFiveArgs) }
		, { 6 , EMITTER_CAST(emitSixArgs) }
		, { 7 , EMITTER_CAST(emitSevenArgs) }
		, { 8 , EMITTER_CAST(emitEightArgs) }
	};

	PFS_ASSERT(count);
	*count = sizeof(local_emitter_mapping)/sizeof(local_emitter_mapping[0]) ;
	return &local_emitter_mapping[0];
}

class LocalPetaloidDetector : public pfs::petaloid
{
public:
	LocalPetaloidDetector() : pfs::petaloid("LocalPetaloidDetector") {}
	virtual ~LocalPetaloidDetector() {}
	virtual const pfs::emitter_mapping* getEmitters(int *count) { PFS_UNUSED(count); return NULL; }
	virtual const pfs::detector_mapping* getDetectors(int *count);

public: /*slots*/
	void onZeroArg();
	void onOneArg(bool ok);
	void onTwoArgs(bool ok, char ch);
	void onThreeArgs(bool ok, char, short i);
	void onFourArgs(bool ok, char, short, int i);
	void onFiveArgs(bool ok, char, short, int, long i);
	void onSixArgs(bool ok, char, short, int, long, const char*);
	void onSevenArgs(bool ok, char, short, int, long, const char*, const pfs::string&);
	void onEightArgs(bool ok, char, short, int, long, long long, const char*, const pfs::string&);
};

inline void LocalPetaloidDetector::onZeroArg()
{
	TEST_OK2(true == true, "onZeroArg()");
}

inline void LocalPetaloidDetector::onOneArg(bool ok)
{
	TEST_OK2(ok == true, "onOneArg(bool)");
}

inline void LocalPetaloidDetector::onTwoArgs(bool ok, char ch)
{
	TEST_OK2(ok == true && ch == 'c', "onTwoArgs(true, 'c')");
}

inline void LocalPetaloidDetector::onThreeArgs(bool ok, char, short i)
{
	TEST_OK2(ok == true && i == PFS_SHORT_MAX, "onThreeArgs(bool,..., PFS_SHORT_MAX)");
}

inline void LocalPetaloidDetector::onFourArgs(bool ok, char, short, int i)
{
	TEST_OK2(ok == true && i == PFS_INT_MAX, "onFourArgs(bool,..., PFS_INT_MAX)");
}

inline void LocalPetaloidDetector::onFiveArgs(bool ok, char, short, int, long i)
{
	TEST_OK2(ok == true && i == PFS_INT_MAX, "onFiveArgs(bool,... PFS_INT_MAX)");
}

inline void LocalPetaloidDetector::onSixArgs(bool ok, char, short, int, long, const char *hello)
{
	TEST_OK2(ok == true && strcmp("Hello, World!", hello) == 0, "onSixArgs(bool,...\"Hello, World!\")");
}

inline void LocalPetaloidDetector::onSevenArgs(bool ok, char, short, int, long, const char*, const pfs::string &hello)
{
	TEST_OK2(ok == true && hello == pfs::string("Hello, Unicode World!"), "onSevenArgs(bool,..., pfs::string(\"Hello, Unicode World!\")");
}

inline void LocalPetaloidDetector::onEightArgs(bool ok, char, short, int, long, long long i, const char*, const pfs::string&)
{
	TEST_OK2(ok == true && i == PFS_LONG_MAX, "onEightArgs(bool,..., PFS_LONG_MAX,...)");
}

const pfs::detector_mapping* LocalPetaloidDetector::getDetectors(int *count)
{
	static pfs::detector_mapping local_detector_mapping[] = {
		  { 0 , DETECTOR_CAST(LocalPetaloidDetector::onZeroArg) }
		, { 1 , DETECTOR_CAST(LocalPetaloidDetector::onOneArg) }
		, { 2 , DETECTOR_CAST(LocalPetaloidDetector::onTwoArgs) }
		, { 3 , DETECTOR_CAST(LocalPetaloidDetector::onThreeArgs) }
		, { 4 , DETECTOR_CAST(LocalPetaloidDetector::onFourArgs) }
		, { 5 , DETECTOR_CAST(LocalPetaloidDetector::onFiveArgs) }
		, { 6 , DETECTOR_CAST(LocalPetaloidDetector::onSixArgs) }
		, { 7 , DETECTOR_CAST(LocalPetaloidDetector::onSevenArgs) }
		, { 8 , DETECTOR_CAST(LocalPetaloidDetector::onEightArgs) }
	};

	PFS_ASSERT(count);
	*count = sizeof(local_detector_mapping)/sizeof(local_detector_mapping[0]) ;
	return &local_detector_mapping[0];
}

static pfs::sepaloid::mapping_type app_mapping[] = {
	  { 0 , new pfs::sigslot_mapping0_t, _l1("ZeroArg()") }
	, { 1 , new pfs::sigslot_mapping1_t<bool>, _l1("OneArg(bool b)\n\t boolean value") }
	, { 2 , new pfs::sigslot_mapping2_t<bool, char>, _l1("TwoArgs(bool b, char ch)") }
	, { 3 , new pfs::sigslot_mapping3_t<bool, char, short>, _l1("ThreeArgs(bool b, char ch, short n)") }
	, { 4 , new pfs::sigslot_mapping4_t<bool, char, short, int>, _l1("FourArgs description") }
	, { 5 , new pfs::sigslot_mapping5_t<bool, char, short, int, long>, _l1("FiveArgs description") }
	, { 6 , new pfs::sigslot_mapping6_t<bool, char, short, int, long, const char*>, _l1("SixArgs description") }
	, { 7 , new pfs::sigslot_mapping7_t<bool, char, short, int, long, const char*, const pfs::string &>, _l1("SevenArgs description") }
	, { 8 , new pfs::sigslot_mapping8_t<bool, char, short, int, long, long long, const char*, const pfs::string &>, _l1("EightArgs description") }
};

int main(int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);
    BEGIN_TESTS(14);

    LocalPetaloidEmitter * emitter = new LocalPetaloidEmitter;
    LocalPetaloidDetector * detector = new LocalPetaloidDetector;

    pfs::sepaloid sepaloid(app_mapping, sizeof(app_mapping)/sizeof(app_mapping[0]));
    TEST_OK_X(sepaloid.registerLocalPetaloid(emitter), sepaloid.logErrors());
    TEST_OK_X(sepaloid.registerLocalPetaloid(detector), sepaloid.logErrors());
    TEST_OK_X(sepaloid.registerPetaloidForPath(_u8("../libpetaloid-tmpl.so")), sepaloid.logErrors());
    sepaloid.addSearchPath(_u8("."));
    sepaloid.addSearchPath(_u8(".."));
    TEST_OK_X(!sepaloid.registerPetaloidForName(_u8("petaloid-tmpl")), sepaloid.logErrors());
    sepaloid.connectAll();

    TEST_OK(sepaloid.count() == 3);
//    pfs::sepaloid::const_iterator it = sepaloid.cbegin();
//    pfs::sepaloid::const_iterator itEnd = sepaloid.cend();
//
//    pfs::debug("List of registered petaloids:");
//    for (int i = 0; it != itEnd; ++it, ++i) {
//    	pfs::string m(pfs::safeformat("pfs::petaloid %02d: %s")(i)(it->name())());
//    	pfs::debug(m);
//    }

    emitter->emitZeroArg   ();
    emitter->emitOneArg    (true);
    emitter->emitTwoArgs   (true, 'c');
    emitter->emitThreeArgs (true, 'c', PFS_SHORT_MAX);
    emitter->emitFourArgs  (true, 'c', PFS_SHORT_MAX, PFS_INT_MAX);
    emitter->emitFiveArgs  (true, 'c', PFS_SHORT_MAX, PFS_INT_MAX, PFS_INT_MAX);
    emitter->emitSixArgs   (true, 'c', PFS_SHORT_MAX, PFS_INT_MAX, PFS_INT_MAX, "Hello, World!");
    emitter->emitSevenArgs (true, 'c', PFS_SHORT_MAX, PFS_INT_MAX, PFS_INT_MAX, "Hello, World!", _u8("Hello, Unicode World!"));
    emitter->emitEightArgs (true, 'c', PFS_SHORT_MAX, PFS_INT_MAX, PFS_INT_MAX, PFS_LONG_MAX, "Hello, World!", _u8("Hello, Unicode World!"));

    // This calls are optional - will be executed while sepaloid destruction
    sepaloid.disconnectAll();
    sepaloid.unregisterAll();

    END_TESTS;
}

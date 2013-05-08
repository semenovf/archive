#include <cwt/test.h>
#include <cwt/sepaloid.hpp>
#include <cstring>

using namespace cwt;

class LocalPetaloidEmitter : public Petaloid
{
public:
	LocalPetaloidEmitter() : Petaloid("LocalPetalodEmitter") {}
	virtual ~LocalPetaloidEmitter() {}
	virtual const EmitterMapping* getEmitters(int *count);
	virtual const DetectorMapping* getDetectors(int *count) { CWT_UNUSED(count); return NULL; }

public: /*signal*/
	signal0<> emitZeroArg;
	signal1<bool> emitOneArg;
	signal2<bool, char> emitTwoArgs;
	signal3<bool, char, short> emitThreeArgs;
	signal4<bool, char, short, int> emitFourArgs;
	signal5<bool, char, short, int, long> emitFiveArgs;
	signal6<bool, char, short, int, long, const char*> emitSixArgs;
	signal7<bool, char, short, int, long, const char*, const String&> emitSevenArgs;
	signal8<bool, char, short, int, long, long long, const char*, const String&> emitEightArgs;
};

const EmitterMapping* LocalPetaloidEmitter::getEmitters(int *count)
{
	static EmitterMapping local_emitter_mapping[] = {
		  { "ZeroArg"   , EMITTER_CAST(emitZeroArg) }
		, { "OneArg"    , EMITTER_CAST(emitOneArg) }
		, { "TwoArgs"   , EMITTER_CAST(emitTwoArgs) }
		, { "ThreeArgs" , EMITTER_CAST(emitThreeArgs) }
		, { "FourArgs"  , EMITTER_CAST(emitFourArgs) }
		, { "FiveArgs"  , EMITTER_CAST(emitFiveArgs) }
		, { "SixArgs"   , EMITTER_CAST(emitSixArgs) }
		, { "SevenArgs" , EMITTER_CAST(emitSevenArgs) }
		, { "EightArgs" , EMITTER_CAST(emitEightArgs) }
	};

	CWT_ASSERT(count);
	*count = sizeof(local_emitter_mapping)/sizeof(local_emitter_mapping[0]) ;
	return &local_emitter_mapping[0];
}

class LocalPetaloidDetector : public Petaloid
{
public:
	LocalPetaloidDetector() : Petaloid("LocalPetaloidDetector") {}
	virtual ~LocalPetaloidDetector() {}
	virtual const EmitterMapping* getEmitters(int *count) { CWT_UNUSED(count); return NULL; }
	virtual const DetectorMapping* getDetectors(int *count);

public: /*slots*/
	void onZeroArg();
	void onOneArg(bool ok);
	void onTwoArgs(bool ok, char ch);
	void onThreeArgs(bool ok, char, short i);
	void onFourArgs(bool ok, char, short, int i);
	void onFiveArgs(bool ok, char, short, int, long i);
	void onSixArgs(bool ok, char, short, int, long, const char*);
	void onSevenArgs(bool ok, char, short, int, long, const char*, const String&);
	void onEightArgs(bool ok, char, short, int, long, long long, const char*, const String&);
};

inline void LocalPetaloidDetector::onZeroArg()
{
	CWT_TEST_OK2(true == true, "onZeroArg()");
}

inline void LocalPetaloidDetector::onOneArg(bool ok)
{
	CWT_TEST_OK2(ok == true, "onOneArg(bool)");
}

inline void LocalPetaloidDetector::onTwoArgs(bool ok, char ch)
{
	CWT_TEST_OK2(ok == true && ch == 'c', "onTwoArgs(true, 'c')");
}

inline void LocalPetaloidDetector::onThreeArgs(bool ok, char, short i)
{
	CWT_TEST_OK2(ok == true && i == CWT_SHORT_MAX, "onThreeArgs(bool,..., CWT_SHORT_MAX)");
}

inline void LocalPetaloidDetector::onFourArgs(bool ok, char, short, int i)
{
	CWT_TEST_OK2(ok == true && i == CWT_INT_MAX, "onFourArgs(bool,..., CWT_INT_MAX)");
}

inline void LocalPetaloidDetector::onFiveArgs(bool ok, char, short, int, long i)
{
	CWT_TEST_OK2(ok == true && i == CWT_INT_MAX, "onFiveArgs(bool,... CWT_INT_MAX)");
}

inline void LocalPetaloidDetector::onSixArgs(bool ok, char, short, int, long, const char *hello)
{
	CWT_TEST_OK2(ok == true && strcmp("Hello, World!", hello) == 0, "onSixArgs(bool,...\"Hello, World!\")");
}

inline void LocalPetaloidDetector::onSevenArgs(bool ok, char, short, int, long, const char*, const String &hello)
{
	CWT_TEST_OK2(ok == true && hello == _U("Hello, Unicode World!"), "onSevenArgs(bool,..., String(\"Hello, Unicode World!\")");
}

inline void LocalPetaloidDetector::onEightArgs(bool ok, char, short, int, long, long long i, const char*, const String&)
{
	CWT_TEST_OK2(ok == true && i == CWT_LONG_MAX, "onEightArgs(bool,..., CWT_LONG_MAX,...)");
}

const DetectorMapping* LocalPetaloidDetector::getDetectors(int *count)
{
	static DetectorMapping local_detector_mapping[] = {
		  { "ZeroArg"   , DETECTOR_CAST(LocalPetaloidDetector::onZeroArg) }
		, { "OneArg"    , DETECTOR_CAST(LocalPetaloidDetector::onOneArg) }
		, { "TwoArgs"   , DETECTOR_CAST(LocalPetaloidDetector::onTwoArgs) }
		, { "ThreeArgs" , DETECTOR_CAST(LocalPetaloidDetector::onThreeArgs) }
		, { "FourArgs"  , DETECTOR_CAST(LocalPetaloidDetector::onFourArgs) }
		, { "FiveArgs"  , DETECTOR_CAST(LocalPetaloidDetector::onFiveArgs) }
		, { "SixArgs"   , DETECTOR_CAST(LocalPetaloidDetector::onSixArgs) }
		, { "SevenArgs" , DETECTOR_CAST(LocalPetaloidDetector::onSevenArgs) }
		, { "EightArgs" , DETECTOR_CAST(LocalPetaloidDetector::onEightArgs) }
	};

	CWT_ASSERT(count);
	*count = sizeof(local_detector_mapping)/sizeof(local_detector_mapping[0]) ;
	return &local_detector_mapping[0];
}

static Sepaloid::Mapping app_mapping[] = {
	  { "ZeroArg",   new sigslot_mapping0_t, "ZeroArg()" }
	, { "OneArg",    new sigslot_mapping1_t<bool>, "OneArg(bool b)\n\t boolean value" }
	, { "TwoArgs",   new sigslot_mapping2_t<bool, char>, "TwoArgs(bool b, char ch)" }
	, { "ThreeArgs", new sigslot_mapping3_t<bool, char, short>, "ThreeArgs(bool b, char ch, short n)" }
	, { "FourArgs",  new sigslot_mapping4_t<bool, char, short, int>, "FourArgs description" }
	, { "FiveArgs",  new sigslot_mapping5_t<bool, char, short, int, long>, "FiveArgs description" }
	, { "SixArgs",   new sigslot_mapping6_t<bool, char, short, int, long, const char*>, "SixArgs description" }
	, { "SevenArgs", new sigslot_mapping7_t<bool, char, short, int, long, const char*, const String&>, "SevenArgs description" }
	, { "EightArgs", new sigslot_mapping8_t<bool, char, short, int, long, long long, const char*, const String&>, "EightArgs description"}
};

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(14);

    LocalPetaloidEmitter *emitter = new LocalPetaloidEmitter;
    LocalPetaloidDetector *detector = new LocalPetaloidDetector;

    Sepaloid sepaloid(app_mapping, sizeof(app_mapping)/sizeof(app_mapping[0]));
    CWT_TEST_OK(sepaloid.registerLocalPetaloid(*emitter));
    CWT_TEST_OK(sepaloid.registerLocalPetaloid(*detector));
    CWT_TEST_OK(sepaloid.registerPetaloidForPath(_U("libpetaloid-tmpl.so")));
    sepaloid.addSearchPath(_U("."));
    CWT_TEST_OK(sepaloid.registerPetaloidForName(_U("petaloid-tmpl")));
    sepaloid.connectAll();

    CWT_TEST_FAIL(sepaloid.count() == 4);
    Sepaloid::const_iterator it = sepaloid.cbegin();
    Sepaloid::const_iterator itEnd = sepaloid.cend();

    Logger::debug("List of registered petaloids:");
    for (int i = 0; it != itEnd; ++it, ++i) {
    	Logger::debug("Petaloid %02d: %ls", i, it->name().utf16());
    }

    emitter->emitZeroArg   ();
    emitter->emitOneArg    (true);
    emitter->emitTwoArgs   (true, 'c');
    emitter->emitThreeArgs (true, 'c', CWT_SHORT_MAX);
    emitter->emitFourArgs  (true, 'c', CWT_SHORT_MAX, CWT_INT_MAX);
    emitter->emitFiveArgs  (true, 'c', CWT_SHORT_MAX, CWT_INT_MAX, CWT_INT_MAX);
    emitter->emitSixArgs   (true, 'c', CWT_SHORT_MAX, CWT_INT_MAX, CWT_INT_MAX, "Hello, World!");
    emitter->emitSevenArgs (true, 'c', CWT_SHORT_MAX, CWT_INT_MAX, CWT_INT_MAX, "Hello, World!", _U("Hello, Unicode World!"));
    emitter->emitEightArgs (true, 'c', CWT_SHORT_MAX, CWT_INT_MAX, CWT_INT_MAX, CWT_LONG_MAX, "Hello, World!", _U("Hello, Unicode World!"));

    // This calls are optional - will be executed while sepaloid destruction
    sepaloid.disconnectAll();
    sepaloid.unregisterAll();

    CWT_END_TESTS;
}

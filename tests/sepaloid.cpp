#include <cwt/test.h>
#include <cwt/sepaloid.hpp>
#include <cstring>

using namespace cwt;

class LocalPetalodEmitter : public Petaloid
{
public:
	LocalPetalodEmitter() : Petaloid("LocalPetalodEmitter") {}
	virtual ~LocalPetalodEmitter() {}
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

const EmitterMapping* LocalPetalodEmitter::getEmitters(int *count)
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

class LocalPetalodDetector : public Petaloid
{
public:
	LocalPetalodDetector() : Petaloid("LocalPetalodDetector") {}
	virtual ~LocalPetalodDetector() {}
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

inline void LocalPetalodDetector::onZeroArg()
{
	CWT_TEST_OK2(true == true, "onZeroArg()");
}

inline void LocalPetalodDetector::onOneArg(bool ok)
{
	CWT_TEST_OK2(ok == true, "onOneArg(bool)");
}

inline void LocalPetalodDetector::onTwoArgs(bool ok, char ch)
{
	CWT_TEST_OK2(ok == true && ch == 'c', "onTwoArgs(true, 'c')");
}

inline void LocalPetalodDetector::onThreeArgs(bool ok, char, short i)
{
	CWT_TEST_OK2(ok == true && i == CWT_SHORT_MAX, "onThreeArgs(bool,..., CWT_SHORT_MAX)");
}

inline void LocalPetalodDetector::onFourArgs(bool ok, char, short, int i)
{
	CWT_TEST_OK2(ok == true && i == CWT_INT_MAX, "onFourArgs(bool,..., CWT_INT_MAX)");
}

inline void LocalPetalodDetector::onFiveArgs(bool ok, char, short, int, long i)
{
	CWT_TEST_OK2(ok == true && i == CWT_INT_MAX, "onFiveArgs(bool,... CWT_INT_MAX)");
}

inline void LocalPetalodDetector::onSixArgs(bool ok, char, short, int, long, const char *hello)
{
	CWT_TEST_OK2(ok == true && strcmp("Hello, World!", hello) == 0, "onSixArgs(bool,...\"Hello, World!\")");
}

inline void LocalPetalodDetector::onSevenArgs(bool ok, char, short, int, long, const char*, const String &hello)
{
	CWT_TEST_OK2(ok == true && hello == _U("Hello, Unicode World!"), "onSevenArgs(bool,..., String(\"Hello, Unicode World!\"");
}

inline void LocalPetalodDetector::onEightArgs(bool ok, char, short, int, long, long long i, const char*, const String&)
{
	CWT_TEST_OK2(ok == true && i == CWT_LONG_MAX, "onEightArgs(bool,..., CWT_LONG_MAX,...)");
}

const DetectorMapping* LocalPetalodDetector::getDetectors(int *count)
{
	static DetectorMapping local_detector_mapping[] = {
		  { "ZeroArg"   , DETECTOR_CAST(LocalPetalodDetector::onZeroArg) }
		, { "OneArg"    , DETECTOR_CAST(LocalPetalodDetector::onOneArg) }
		, { "TwoArgs"   , DETECTOR_CAST(LocalPetalodDetector::onTwoArgs) }
		, { "ThreeArgs" , DETECTOR_CAST(LocalPetalodDetector::onThreeArgs) }
		, { "FourArgs"  , DETECTOR_CAST(LocalPetalodDetector::onFourArgs) }
		, { "FiveArgs"  , DETECTOR_CAST(LocalPetalodDetector::onFiveArgs) }
		, { "SixArgs"   , DETECTOR_CAST(LocalPetalodDetector::onSixArgs) }
		, { "SevenArgs" , DETECTOR_CAST(LocalPetalodDetector::onSevenArgs) }
		, { "EightArgs" , DETECTOR_CAST(LocalPetalodDetector::onEightArgs) }
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
    CWT_BEGIN_TESTS(9);

    LocalPetalodEmitter *emitter = new LocalPetalodEmitter;
    LocalPetalodDetector *detector = new LocalPetalodDetector;

    Sepaloid sepaloid(app_mapping, sizeof(app_mapping)/sizeof(app_mapping[0]));
    sepaloid.registerLocalPetaloid(*emitter);
    sepaloid.registerLocalPetaloid(*detector);
    sepaloid.connectAll();

    emitter->emitZeroArg   ();
    emitter->emitOneArg    (true);
    emitter->emitTwoArgs   (true, 'c');
    emitter->emitThreeArgs (true, 'c', CWT_SHORT_MAX);
    emitter->emitFourArgs  (true, 'c', CWT_SHORT_MAX, CWT_INT_MAX);
    emitter->emitFiveArgs  (true, 'c', CWT_SHORT_MAX, CWT_INT_MAX, CWT_INT_MAX);
    emitter->emitSixArgs   (true, 'c', CWT_SHORT_MAX, CWT_INT_MAX, CWT_INT_MAX, "Hello, World!");
    emitter->emitSevenArgs (true, 'c', CWT_SHORT_MAX, CWT_INT_MAX, CWT_INT_MAX, "Hello, World!", _U("Hello, Unicode World!"));
    emitter->emitEightArgs (true, 'c', CWT_SHORT_MAX, CWT_INT_MAX, CWT_INT_MAX, CWT_LONG_MAX, "Hello, World!", _U("Hello, Unicode World!"));

    CWT_END_TESTS;
}

/**
 * @file   petaloid_tmpl.cpp
 * @author wladt
 * @date   Feb 15, 2013 12:03:15 PM
 *
 * @brief
 */

#include <cwt/petaloid.hpp>
#include <cstdio>

using namespace cwt;

class PetaloidTemplate : public cwt::Petaloid
{
public:
	PetaloidTemplate() : Petaloid("PetaloidTemplate") {}
	virtual ~PetaloidTemplate() { CWT_TRACE("PetaloidTemplate::~PetaloidTemplate()"); }
	virtual const EmitterMapping* getEmitters(int *count);
	virtual const DetectorMapping* getDetectors(int *count);

public: /*signal*/
	signal0<> emitZeroArg;
	signal1<bool> emitOneArg;
	signal2<bool, char> emitTwoArgs;
	signal3<bool, char, short> emitThreeArgs;
	signal4<bool, char, short, int> emitFourArgs;
	signal5<bool, char, short, int, long> emitFiveArgs;
	signal6<bool, char, short, int, long, const char*> emitSixArgs;

public: /*slots*/
	void onZeroArg();
	void onOneArg(bool ok);
	void onTwoArgs(bool ok, char ch);
	void onThreeArgs(bool ok, char, short i);
	void onFourArgs(bool ok, char, short, int i);
	void onFiveArgs(bool ok, char, short, int, long i);
	void onSixArgs(bool ok, char, short, int, long, const char*);

};

const EmitterMapping* PetaloidTemplate::getEmitters(int *count)
{
	static EmitterMapping emitter_mapping[] = {
		  { "ZeroArg"   , EMITTER_CAST(emitZeroArg) }
		, { "OneArg"    , EMITTER_CAST(emitOneArg) }
		, { "TwoArgs"   , EMITTER_CAST(emitTwoArgs) }
		, { "ThreeArgs" , EMITTER_CAST(emitThreeArgs) }
		, { "FourArgs"  , EMITTER_CAST(emitFourArgs) }
		, { "FiveArgs"  , EMITTER_CAST(emitFiveArgs) }
		, { "SixArgs"   , EMITTER_CAST(emitSixArgs) }
	};

	CWT_ASSERT(count);
	*count = sizeof(emitter_mapping)/sizeof(emitter_mapping[0]) ;
	return &emitter_mapping[0];
}


const DetectorMapping* PetaloidTemplate::getDetectors(int *count)
{
	static DetectorMapping detector_mapping[] = {
		  { "ZeroArg"   , DETECTOR_CAST(PetaloidTemplate::onZeroArg) }
		, { "OneArg"    , DETECTOR_CAST(PetaloidTemplate::onOneArg) }
		, { "TwoArgs"   , DETECTOR_CAST(PetaloidTemplate::onTwoArgs) }
		, { "ThreeArgs" , DETECTOR_CAST(PetaloidTemplate::onThreeArgs) }
		, { "FourArgs"  , DETECTOR_CAST(PetaloidTemplate::onFourArgs) }
		, { "FiveArgs"  , DETECTOR_CAST(PetaloidTemplate::onFiveArgs) }
		, { "SixArgs"   , DETECTOR_CAST(PetaloidTemplate::onSixArgs) }
	};

	CWT_ASSERT(count);
	*count = sizeof(detector_mapping)/sizeof(detector_mapping[0]) ;
	return &detector_mapping[0];
}

inline void PetaloidTemplate::onZeroArg()
{
	printf("PetaloidTemplate::onZeroArg()\n");
}

inline void PetaloidTemplate::onOneArg(bool ok)
{
	printf("PetaloidTemplate::onOneArg(%s)\n", ok ? "true" : "false");
}

inline void PetaloidTemplate::onTwoArgs(bool ok, char ch)
{
	printf("PetaloidTemplate::onTwoArgs(%s, '%c')\n", ok ? "true" : "false", ch);
}

inline void PetaloidTemplate::onThreeArgs(bool, char, short)
{
	printf("PetaloidTemplate::onThreeArgs()\n");
}

inline void PetaloidTemplate::onFourArgs(bool, char, short, int)
{
	printf("PetaloidTemplate::onFourArgs()\n");
}

inline void PetaloidTemplate::onFiveArgs(bool, char, short, int, long)
{
	printf("PetaloidTemplate::onFiveArgs()\n");
}

inline void PetaloidTemplate::onSixArgs(bool ok, char, short, int, long, const char *hello)
{
	printf("PetaloidTemplate::onSixArgs(%s, ..., \"%s\")\n", ok ? "true" : "false", hello);
}


CWT_PETALOID_API Petaloid* __petaloid_ctor__(Sepaloid *, const char *, int , char **) { return dynamic_cast<Petaloid*>(new PetaloidTemplate()); }
CWT_PETALOID_API void  __petaloid_dtor__(Petaloid *p) { CWT_ASSERT(p); delete p; }

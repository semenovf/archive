/**
 * @file   petaloid_tmpl.cpp
 * @author wladt
 * @date   Feb 15, 2013 12:03:15 PM
 *
 * @brief
 */

#include <cwt/petaloid.hpp>
#include <cstdio>

class PetaloidTemplate : public cwt::petaloid
{
public:
	PetaloidTemplate() : petaloid("PetaloidTemplate") {}
	virtual ~PetaloidTemplate() { PFS_TRACE("PetaloidTemplate::~PetaloidTemplate()"); }
	virtual const cwt::emitter_mapping * getEmitters(int * count);
	virtual const cwt::detector_mapping * getDetectors(int * count);

public: /*signal*/
	cwt::signal0<> emitZeroArg;
	cwt::signal1<bool> emitOneArg;
	cwt::signal2<bool, char> emitTwoArgs;
	cwt::signal3<bool, char, short> emitThreeArgs;
	cwt::signal4<bool, char, short, int> emitFourArgs;
	cwt::signal5<bool, char, short, int, long> emitFiveArgs;
	cwt::signal6<bool, char, short, int, long, const char*> emitSixArgs;

public: /*slots*/
	void onZeroArg();
	void onOneArg(bool ok);
	void onTwoArgs(bool ok, char ch);
	void onThreeArgs(bool ok, char, short i);
	void onFourArgs(bool ok, char, short, int i);
	void onFiveArgs(bool ok, char, short, int, long i);
	void onSixArgs(bool ok, char, short, int, long, const char*);

};

const cwt::emitter_mapping* PetaloidTemplate::getEmitters(int * count)
{
	static cwt::emitter_mapping emitter_mapping[] = {
		  { 0 , EMITTER_CAST(emitZeroArg) }
		, { 1 , EMITTER_CAST(emitOneArg) }
		, { 2 , EMITTER_CAST(emitTwoArgs) }
		, { 3 , EMITTER_CAST(emitThreeArgs) }
		, { 4 , EMITTER_CAST(emitFourArgs) }
		, { 5 , EMITTER_CAST(emitFiveArgs) }
		, { 6 , EMITTER_CAST(emitSixArgs) }
	};

	PFS_ASSERT(count);
	*count = sizeof(emitter_mapping)/sizeof(emitter_mapping[0]) ;
	return &emitter_mapping[0];
}


const cwt::detector_mapping* PetaloidTemplate::getDetectors(int *count)
{
	static cwt::detector_mapping detector_mapping[] = {
		  { 0 , DETECTOR_CAST(PetaloidTemplate::onZeroArg) }
		, { 1 , DETECTOR_CAST(PetaloidTemplate::onOneArg) }
		, { 2 , DETECTOR_CAST(PetaloidTemplate::onTwoArgs) }
		, { 3 , DETECTOR_CAST(PetaloidTemplate::onThreeArgs) }
		, { 4 , DETECTOR_CAST(PetaloidTemplate::onFourArgs) }
		, { 5 , DETECTOR_CAST(PetaloidTemplate::onFiveArgs) }
		, { 6 , DETECTOR_CAST(PetaloidTemplate::onSixArgs) }
	};

	PFS_ASSERT(count);
	*count = sizeof(detector_mapping)/sizeof(detector_mapping[0]) ;
	return & detector_mapping[0];
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


CWT_PETALOID_API cwt::petaloid * __petaloid_ctor__(const char *, int , char **) { return dynamic_cast<cwt::petaloid *>(new PetaloidTemplate()); }
CWT_PETALOID_API void  __petaloid_dtor__(cwt::petaloid * p) { PFS_ASSERT(p); delete p; }

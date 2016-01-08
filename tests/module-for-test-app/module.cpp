/**
 * @file   petaloid_tmpl.cpp
 * @author wladt
 * @date   Feb 15, 2013 12:03:15 PM
 *
 * @brief
 */

#include <cstdio>
#include <pfs/module.hpp>

namespace test {

class module : public pfs::module
{
public:
	module () : pfs::module()
	{}

	virtual bool on_start (pfs::notification & nx)
	{
		return true;
	}

	virtual bool on_finish ()
	{
		return true;
	}

	PFS_MODULE_EMITTERS_BEGIN
		  PFS_MODULE_EMITTER(0, emitZeroArg)
		, PFS_MODULE_EMITTER(1, emitOneArg)
		, PFS_MODULE_EMITTER(2, emitTwoArgs)
		, PFS_MODULE_EMITTER(3, emitThreeArgs)
		, PFS_MODULE_EMITTER(4, emitFourArgs)
		, PFS_MODULE_EMITTER(5, emitFiveArgs)
		, PFS_MODULE_EMITTER(6, emitSixArgs)
	PFS_MODULE_EMITTERS_END

	PFS_MODULE_DETECTORS_BEGIN
		  PFS_MODULE_DETECTOR(0, module::onZeroArg)
		, PFS_MODULE_DETECTOR(1, module::onOneArg)
		, PFS_MODULE_DETECTOR(2, module::onTwoArgs)
		, PFS_MODULE_DETECTOR(3, module::onThreeArgs)
		, PFS_MODULE_DETECTOR(4, module::onFourArgs)
		, PFS_MODULE_DETECTOR(5, module::onFiveArgs)
		, PFS_MODULE_DETECTOR(6, module::onSixArgs)
	PFS_MODULE_DETECTORS_END

public: /*signal*/
	pfs::signal0<> emitZeroArg;
	pfs::signal1<bool> emitOneArg;
	pfs::signal2<bool, char> emitTwoArgs;
	pfs::signal3<bool, char, short> emitThreeArgs;
	pfs::signal4<bool, char, short, int> emitFourArgs;
	pfs::signal5<bool, char, short, int, long> emitFiveArgs;
	pfs::signal6<bool, char, short, int, long, const char*> emitSixArgs;

public: /*slots*/
	void onZeroArg   ();
	void onOneArg    (bool ok);
	void onTwoArgs   (bool ok, char ch);
	void onThreeArgs (bool ok, char, short i);
	void onFourArgs  (bool ok, char, short, int i);
	void onFiveArgs  (bool ok, char, short, int, long i);
	void onSixArgs   (bool ok, char, short, int, long, const char*);
};

inline void module::onZeroArg ()
{
	printf("module::onZeroArg()\n");
}

inline void module::onOneArg (bool ok)
{
	printf("module::onOneArg(%s)\n", ok ? "true" : "false");
}

inline void module::onTwoArgs (bool ok, char ch)
{
	printf("module::onTwoArgs(%s, '%c')\n", ok ? "true" : "false", ch);
}

inline void module::onThreeArgs (bool, char, short)
{
	printf("module::onThreeArgs()\n");
}

inline void module::onFourArgs (bool, char, short, int)
{
	printf("module::onFourArgs()\n");
}

inline void module::onFiveArgs (bool, char, short, int, long)
{
	printf("module::onFiveArgs()\n");
}

inline void module::onSixArgs (bool ok, char, short, int, long, const char *hello)
{
	printf("module::onSixArgs(%s, ..., \"%s\")\n", ok ? "true" : "false", hello);
}

} // test

EXTERN_C_BEGIN
PFS_MODULE_API pfs::module * __module_ctor__ (const char *, int /*argc*/, const char * /*argv*/[])
{
	test::module * m = new test::module;
	return m;
}

PFS_MODULE_API void  __module_dtor__ (pfs::module * m)
{
	delete m;
}
EXTERN_C_END


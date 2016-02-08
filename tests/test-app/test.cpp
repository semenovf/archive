#include <cstring>
#include <pfs/test/test.hpp>
#include <pfs/logger.hpp>
#include <pfs/safeformat.hpp>
#include <pfs/app.hpp>

class module : public pfs::module
{
public:
	module () : pfs::module()
	{}

	~module ()
	{}

	virtual bool on_start ()
	{
	    emitZeroArg   ();
	    emitOneArg    (true);
	    emitTwoArgs   (true, 'c');
	    emitThreeArgs (true, 'c', pfs::max_value<short>());
	    emitFourArgs  (true, 'c', pfs::max_value<short>(), pfs::max_value<int>());
	    emitFiveArgs  (true, 'c', pfs::max_value<short>(), pfs::max_value<int>(), pfs::max_value<int>());
	    emitSixArgs   (true, 'c', pfs::max_value<short>(), pfs::max_value<int>(), pfs::max_value<int>(), "Hello, World!");
//	    emitSevenArgs (true, 'c', pfs::max_value<short>(), pfs::max_value<int>(), pfs::max_value<int>(), "Hello, World!", _u8("Hello, Unicode World!"));
//	    emitEightArgs (true, 'c', pfs::max_value<short>(), pfs::max_value<int>(), pfs::max_value<int>(), pfs::max_value<long>(), "Hello, World!", _u8("Hello, Unicode World!"));

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
	TEST_OK2(true == true, "onZeroArg()");
}

inline void module::onOneArg  (bool ok)
{
	TEST_OK2(ok == true, "onOneArg(bool)");
}

inline void module::onTwoArgs (bool ok, char ch)
{
	TEST_OK2(ok == true && ch == 'c', "onTwoArgs(true, 'c')");
}

inline void module::onThreeArgs (bool ok, char, short i)
{
	TEST_OK2(ok == true && i == pfs::max_value<short>(), "onThreeArgs(bool,..., PFS_SHORT_MAX)");
}

inline void module::onFourArgs (bool ok, char, short, int i)
{
	TEST_OK2(ok == true && i == pfs::max_value<int>(), "onFourArgs(bool,..., PFS_INT_MAX)");
}

inline void module::onFiveArgs (bool ok, char, short, int, long i)
{
	TEST_OK2(ok == true && i == pfs::max_value<int>(), "onFiveArgs(bool,... PFS_INT_MAX)");
}

inline void module::onSixArgs (bool ok, char, short, int, long, const char *hello)
{
	TEST_OK2(ok == true && strcmp("Hello, World!", hello) == 0, "onSixArgs(bool,...\"Hello, World!\")");
}

//inline void module::onSevenArgs (bool ok, char, short, int, long, const char*, const pfs::string & hello)
//{
//	TEST_OK2(ok == true && hello == pfs::string("Hello, Unicode World!"), "onSevenArgs(bool,..., pfs::string(\"Hello, Unicode World!\")");
//}
//
//inline void module::onEightArgs (bool ok, char, short, int, long, long long i, const char*, const pfs::string &)
//{
//	TEST_OK2(ok == true && i == pfs::max_value<long>(), "onEightArgs(bool,..., PFS_LONG_MAX,...)");
//}

static pfs::dispatcher::api_item_type API[] = {
	  { 0 , new pfs::sigslot_mapping0_t, _u8("ZeroArg()") }
	, { 1 , new pfs::sigslot_mapping1_t<bool>, _u8("OneArg(bool b)\n\t boolean value") }
	, { 2 , new pfs::sigslot_mapping2_t<bool, char>, _u8("TwoArgs(bool b, char ch)") }
	, { 3 , new pfs::sigslot_mapping3_t<bool, char, short>, _u8("ThreeArgs(bool b, char ch, short n)") }
	, { 4 , new pfs::sigslot_mapping4_t<bool, char, short, int>, _u8("FourArgs description") }
	, { 5 , new pfs::sigslot_mapping5_t<bool, char, short, int, long>, _u8("FiveArgs description") }
	, { 6 , new pfs::sigslot_mapping6_t<bool, char, short, int, long, const char*>, _u8("SixArgs description") }
//	, { 7 , new pfs::sigslot_mapping7_t<bool, char, short, int, long, const char*, const pfs::string &>, _u8("SevenArgs description") }
//	, { 8 , new pfs::sigslot_mapping8_t<bool, char, short, int, long, long long, const char*, const pfs::string &>, _u8("EightArgs description") }
};

int main (int argc, char *argv[])
{
    PFS_UNUSED(argc);
    BEGIN_TESTS(12);

    pfs::dispatcher dispatcher(API, sizeof(API)/sizeof(API[0]));
    dispatcher.add_search_path(pfs::fs::path("."));

    TEST_OK(dispatcher.register_module_for_name(_u8("module-for-test-app")));
    TEST_OK(dispatcher.register_local_module(new module, _u8("mod-local-for-test-app")));
    TEST_OK(!dispatcher.register_module_for_name(_u8("module-for-test-app-nonexistence")));

    TEST_OK(dispatcher.count() == 2);

    pfs::app app;//(_u8(argv[0]));
    TEST_OK(app.exec(dispatcher) == 0);

    END_TESTS;
}

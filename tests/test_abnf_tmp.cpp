#include "/home/wladt/build/unix/x86/debug/1.cpp"

#include <cwt/string.hpp>
#include <cwt/fsm.hpp>
using namespace cwt;

bool parse (String text)
{
	Fsm<String> fsm; //(rulelist_fsm, nullptr);

	if (fsm.exec(0, text.begin(), text.end()) >= 0) {
		return true;
	}

	return false;
}

int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	return parse("Hello") ? EXIT_SUCCESS : EXIT_FAILURE;
}


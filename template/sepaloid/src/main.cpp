#include <cwt/app.hpp>

#define _INCLUDE_SEPALOID_MAPPING
#include "api.hpp"

int main (int argc, char **argv)
{
    PFS_CHECK_SIZEOF_TYPES;

    cwt::sepaloid sepaloid(API, sizeof(API)/sizeof(API[0]));
    sepaloid.addSearchPath(_l1("."));

    sepaloid.registerPetaloidForName(_l1("hg-booker-core"));
    sepaloid.registerPetaloidForName(_l1("hg-booker-db"));
    sepaloid.registerPetaloidForName(_l1("hg-booker-io"));
    sepaloid.registerPetaloidForName(_l1("hg-booker-logger"));
    sepaloid.registerPetaloidForName(_l1("hg-booker-prefs"));

#ifdef _SIMULATOR
    sepaloid.registerPetaloidForName(_l1("hg-booker-simulator"));
#endif

    cwt::petaloid * uiPetaloid = sepaloid.registerPetaloidForName(_l1("hg-booker-ui-qt")); // load UI
    sepaloid.setMasterPetaloid(uiPetaloid);

	cwt::app app(argc, argv);
	return app.exec(sepaloid);
}

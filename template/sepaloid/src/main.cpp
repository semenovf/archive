#include <pfs/app.hpp>

#define _INCLUDE_SEPALOID_MAPPING
#include "api.hpp"
#include "app_name.h"

int main (int argc, char **argv)
{
    pfs::sepaloid sepaloid(API, sizeof(API)/sizeof(API[0]));
    sepaloid.addSearchPath(_l1("."));

    sepaloid.registerPetaloidForName(_l1(_APP_NAME "-core"));
    sepaloid.registerPetaloidForName(_l1(_APP_NAME "-db"));
    sepaloid.registerPetaloidForName(_l1(_APP_NAME "-io"));
    sepaloid.registerPetaloidForName(_l1(_APP_NAME "-logger"));
    sepaloid.registerPetaloidForName(_l1(_APP_NAME "-prefs"));

#ifdef _SIMULATOR
    sepaloid.registerPetaloidForName(_l1(_APP_NAME "-simulator"));
#endif

    pfs::petaloid * uiPetaloid = sepaloid.registerPetaloidForName(_l1(_APP_NAME "-ui")); // load UI
    sepaloid.setMasterPetaloid(uiPetaloid);

    pfs::app app(argc, argv);
    return app.exec(sepaloid);
}

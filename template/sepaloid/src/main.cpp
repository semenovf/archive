#include <pfs/app.hpp>

#define INCLUDE_SEPALOID_MAPPING
#include "api.hpp"
#include "app_name.h"

int main (int argc, char **argv)
{
    PFS_UNUSED2(argc, argv);
    pfs::sepaloid sepaloid(API, sizeof(API)/sizeof(API[0]));
    sepaloid.addSearchPath(_l1("."));

    sepaloid.registerPetaloidForName(_l1(APP_NAME "-core"));
    sepaloid.registerPetaloidForName(_l1(APP_NAME "-db"));
    sepaloid.registerPetaloidForName(_l1(APP_NAME "-io"));
    sepaloid.registerPetaloidForName(_l1(APP_NAME "-logger"));
    sepaloid.registerPetaloidForName(_l1(APP_NAME "-prefs"));
    sepaloid.registerPetaloidForName(_l1(APP_NAME "-simulator"));

    pfs::petaloid * uiPetaloid = sepaloid.registerPetaloidForName(_l1(APP_NAME "-ui")); // load UI
    sepaloid.setMasterPetaloid(uiPetaloid);

    pfs::app app(_u8(APP_NAME));
    return app.exec(sepaloid);
}

#include <iostream>
#include "pfs/platform.hpp"

using std::cout;
using std::endl;

int main (int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);

    cout << "timezone: " << pfs::platform::timezone() << endl
         << "UTC offset in seconds: " << pfs::platform::offset_utc() << endl
         << "UTC offset string: " << pfs::timezone::offset_to_string(pfs::platform::offset_utc()) << endl;
            
    return 0;
}
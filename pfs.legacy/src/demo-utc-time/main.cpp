#include <iostream>
#include <pfs/datetime.hpp>

using std::cout;
using std::endl;

int main (int argc, char * argv[])
{
    PFS_UNUSED2(argc, argv);

    cout << "timezone: " << pfs::timezone_name() << endl
         << "UTC offset in seconds: " << pfs::offset_utc() << endl
         << "UTC offset string: " << pfs::timezone::offset_to_string(pfs::offset_utc()) << endl;
            
    return 0;
}
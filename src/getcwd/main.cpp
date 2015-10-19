#include <iostream>
#include <pfs/fs.hpp>

using std::cout;
using std::endl;

int main (int argc, char * argv[])
{
	PFS_UNUSED2(argc, argv);

	pfs::fs fs;
	pfs::string cwd = fs.currentDirectory();

	if (cwd.isNull()) {
		fs.logErrors();
	} else {
		cout << cwd << endl;
	}

    return 0;
}

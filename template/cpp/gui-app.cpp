#include <iostream>

using std::cout;
using std::endl;

int main (int argc, char * argv[])
{
    (void)argc; // avoid 'unused ...' warning
    cout << "Hello, " << argv[0] << '!' << endl;
    return 0;
}

#include <iostream>
#include "metric.hpp"

using std::cout;
using std::endl;

struct A
{
	~A () { cout << "~A()" << endl; }
};

struct B
{
	~B () { cout << "~B()" << endl; }
};

struct C
{
	C (int) {}
	~C () { cout << "~C()" << endl; }
};


int main (int argc, char * argv[])
{
	metric a = make_metric<A>();
	metric b = make_metric<B>();
	metric c = make_metric<C, int>(1);

    return 0;
}

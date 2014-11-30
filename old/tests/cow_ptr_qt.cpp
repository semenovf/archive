/**
 * @file cow_ptr_qt.cpp
 * @author wladt
 * @date Nov 22, 2013
 */

#include <pfs.hpp>
#include <QByteArray>
#include <QVector>
#include <iostream>

void test_bytearray()
{
	// Taken from http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Copy-on-write
	QByteArray s1("Hello");
	QByteRef c = s1[4];  // Non-const detachment does nothing here
	QByteArray s2(s1); // Lazy-copy, shared state
	c = '!';

	if(s1[4] == s2[4] && s2[4] == '!') {
		std::cout << "COW for QByteArray Failed" << std::endl;
	} else {
		std::cout << "COW for QByteArray is Ok" << std::endl;
	}

	std::cout << "s1=" << s1.data() << std::endl;
	std::cout << "s2=" << s2.data() << std::endl;
}

void test_vector()
{
	QVector<char> s1;
	s1.append('H');
	s1.append('e');
	s1.append('l');
	s1.append('l');
	s1.append('o');

	char & c = s1[4];  // Non-const detachment does nothing here
	QVector<char> s2(s1); // Lazy-copy, shared state
	c = '!';

	if(s1[4] == s2[4] && s2[4] == '!') {
		std::cout << "COW for QVector failed" << std::endl;
	} else {
		std::cout << "COW for QVector is Ok" << std::endl;
	}

	s1.append(0);
	s2.append(0);
	std::cout << "s1=" << s1.data() << std::endl;
	std::cout << "s2=" << s2.data() << std::endl;
}

int main(int argc, char *argv[])
{
	PFS_UNUSED2(argc, argv);
	test_bytearray();
	test_vector();
}

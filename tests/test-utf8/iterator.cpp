/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <pfs/test/test.hpp>
#include <pfs/utf8/iterator.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include "test_data.hpp"

using std::cout;
using std::endl;

template <typename Iterator>
void __test_forward (const char * itertype)
{
	int ntests = sizeof(data)/sizeof(data[0]);
	ADD_TESTS(ntests);

	for (int i = 0; i < ntests; ++i) {
		pfs::utf8::iterator<Iterator> it(iter_cast<Iterator>(data[i].text));
		pfs::utf8::iterator<Iterator> itEnd(iter_cast<Iterator>(data[i].text) + data[i].len);
		size_t count = 0;

		while (it < itEnd) {
			++it;
			++count;
		}

		std::ostringstream desc;

		desc << "Forward iterate using pfs::utf8::iterator<" << itertype << ">."
				<< " String `" << data[i].name << "'."
				<< " Number of unicode chars " << count << ", expected " << data[i].nchars;
		TEST_OK2(count == data[i].nchars, desc.str().c_str());
	}
}

template <typename Iterator>
void __test_backward (const char * itertype)
{
	int ntests = sizeof(data)/sizeof(data[0]);
	ADD_TESTS(ntests);

	for (int i = 0; i < ntests; ++i) {
		pfs::utf8::iterator<Iterator> itBegin(iter_cast<Iterator>(data[i].text));
		pfs::utf8::iterator<Iterator> it(iter_cast<Iterator>(data[i].text) + data[i].len);
		size_t count = 0;

		while (it-- > itBegin) {
			++count;
		}

		std::ostringstream desc;

		desc << "Reverse iterate using pfs::utf8::iterator<" << itertype << ">. String `"
				<< data[i].name
				<< "'. Number of unicode chars "
				<< count
				<< ", expected "
				<< data[i].nchars;
		TEST_OK2(count == data[i].nchars, desc.str().c_str());
	}
}

template <typename Iterator>
void __test_std_advance_forward(const char * itertype)
{
	int ntests = sizeof(data)/sizeof(data[0]);
	ADD_TESTS(ntests * 2);

	for (int i = 0; i < ntests; ++i) {
		pfs::utf8::iterator<Iterator> itBegin(iter_cast<Iterator>(data[i].text));
		pfs::utf8::iterator<Iterator> itEnd(iter_cast<Iterator>(data[i].text) + data[i].len);
		size_t count = 0;

		pfs::utf8::iterator<Iterator> it = itBegin;

		while (it < itEnd) {
			std::advance(it, 1);
			++count;
		}

		std::ostringstream desc;

		desc << "Forward iterate using std::advance(pfs::utf8::iterator<" << itertype << "> &). String `"
				<< data[i].name
				<< "'. Number of unicode chars "
				<< count
				<< ", expected "
				<< data[i].nchars;
		TEST_OK2(count == data[i].nchars, desc.str().c_str());

		it = itBegin;
		std::advance(it, count);
		std::ostringstream desc1;
		desc1 << "Jump to the end by std::advance(pfs::utf8::iterator<" << itertype << "> &)";
		TEST_OK2(it == itEnd, desc1.str().c_str());
	}
}

template <typename Iterator>
void __test_std_advance_backward (const char * itertype)
{
	int ntests = sizeof(data)/sizeof(data[0]);
	ADD_TESTS(ntests * 2);

	for (int i = 0; i < ntests; ++i) {
		pfs::utf8::iterator<Iterator> itBegin(iter_cast<Iterator>(data[i].text));
		pfs::utf8::iterator<Iterator> itEnd(iter_cast<Iterator>(data[i].text) + data[i].len);
		size_t count = 0;

		pfs::utf8::iterator<Iterator> it = itEnd;

		while (it > itBegin) {
			std::advance(it, -1);
			++count;
		}

		std::ostringstream desc;

		desc << "Reverse iterate using std::advance(pfs::utf8::iterator<" << itertype << "> &). String `"
				<< data[i].name
				<< "'. Number of unicode chars "
				<< count
				<< ", expected "
				<< data[i].nchars;
		TEST_OK2(count == data[i].nchars, desc.str().c_str());

		it = itEnd;
		std::advance(it, -count);
		std::ostringstream desc1;
		desc1 << "Jump to the begin by std::advance(pfs::utf8::iterator<" << itertype << "> &)";
		TEST_OK2(it == itBegin, desc1.str().c_str());
	}
}

template <typename Iterator>
void __test_std_reverse_iterator(const char * itertype)
{
	int ntests = sizeof(data)/sizeof(data[0]);
	ADD_TESTS(ntests);

	typedef pfs::utf8::iterator<Iterator> utf8_iterator;
	typedef std::reverse_iterator<pfs::utf8::iterator<Iterator> > utf8_reverse_iterator;

	for (int i = 0; i < ntests; ++i) {
		utf8_reverse_iterator it(utf8_iterator(iter_cast<Iterator>(data[i].text) + data[i].len));
		utf8_reverse_iterator itEnd(utf8_iterator(iter_cast<Iterator>(data[i].text)));
		size_t count = 0;

		while (it < itEnd) {
			++it;
			++count;
		}

		std::ostringstream desc;

		desc << "Iterate forward using std::reverse_iterator<pfs::utf8::iterator<" << itertype << ">>. "
				<< " String `" << data[i].name << "'. "
				<< " Number of unicode chars " << count << ", expected " << data[i].nchars;
		TEST_OK2(count == data[i].nchars, desc.str().c_str());
	}
}

void test_iterator ()
{
	__test_forward<unsigned char *>("unsigned char *");
	__test_forward<const unsigned char *>("const unsigned char *");
	__test_forward<char *>("char *");
	__test_forward<const char *>("const char *");
	__test_forward<std::string::iterator>("std::string::iterator");
	__test_forward<std::string::const_iterator>("std::string::const_iterator");

	__test_backward<unsigned char *>("unsigned char *");
	__test_backward<const unsigned char *>("const unsigned char *");
	__test_backward<char *>("char *");
	__test_backward<const char *>("const char *");
	__test_backward<std::string::iterator>("std::string::iterator");
	__test_backward<std::string::const_iterator>("std::string::const_iterator");

	__test_std_advance_forward<unsigned char *>("unsigned char *");
	__test_std_advance_forward<const unsigned char *>("const unsigned char *");
	__test_std_advance_forward<char *>("char *");
	__test_std_advance_forward<const char *>("const char *");
	__test_std_advance_forward<std::string::iterator>("std::string::iterator");
	__test_std_advance_forward<std::string::const_iterator>("std::string::const_iterator");

	__test_std_advance_backward<unsigned char *>("unsigned char *");
	__test_std_advance_backward<const unsigned char *>("const unsigned char *");
	__test_std_advance_backward<char *>("char *");
	__test_std_advance_backward<const char *>("const char *");
	__test_std_advance_backward<std::string::iterator>("std::string::iterator");
	__test_std_advance_backward<std::string::const_iterator>("std::string::const_iterator");

	__test_std_reverse_iterator<unsigned char *>("unsigned char *");
	__test_std_reverse_iterator<const unsigned char *>("const unsigned char *");
	__test_std_reverse_iterator<char *>("char *");
	__test_std_reverse_iterator<const char *>("const char *");
	__test_std_reverse_iterator<std::string::iterator>("std::string::iterator");
	__test_std_reverse_iterator<std::string::const_iterator>("std::string::const_iterator");
}

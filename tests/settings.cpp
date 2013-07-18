/*
 * settings.cpp
 *
 *  Created on: Jun 26, 2013
 *      Author: wladt
 */

#include "../include/cwt/settings.hpp"
#include <cwt/test.h>

using namespace cwt;

void test_settings_base()
{
	Settings settings;
	settings.setValue(_U("/Image/Width"), 800);
	settings.setValue(_U("/Image/Height"), 600);
	settings.setValue(_U("/Image/Title"), "View from 15th Floor");

	settings.setValue(_U("/Image/IDs/[2]"), 234);
	settings.setValue(_U("/Image/IDs/[3]"), 38793);
	settings.setValue(_U("/Image/IDs/[1]"), 943);
	settings.setValue(_U("/Image/IDs/[0]"), 116);

	CWT_TEST_OK(settings.number(_U("/Image/Width"))  == double(800));
	CWT_TEST_OK(settings.number(_U("/Image/Height")) == double(600));
	CWT_TEST_OK(settings.string(_U("/Image/Title"))  == _U("View from 15th Floor"));

	CWT_TEST_OK(settings.number(_U("/Image/IDs[0]")) == double(116));
	CWT_TEST_OK(settings.number(_U("/Image/IDs[1]")) == double(943));
	CWT_TEST_OK(settings.number(_U("/Image/IDs[2]")) == double(234));
	CWT_TEST_OK(settings.number(_U("/Image/IDs[3]")) == double(38793));

	CWT_TEST_OK(settings[_U("/Image/Width")].number()  == double(800));
	CWT_TEST_OK(settings[_U("/Image/Height")].number() == double(600));
	CWT_TEST_OK(settings[_U("/Image/Title")].string()  == _U("View from 15th Floor"));

	CWT_TEST_OK(settings[_U("/Image/IDs[0]")].integer() == integer_type(116));
	CWT_TEST_OK(settings[_U("/Image/IDs[1]")].number() == number_type(943));
	CWT_TEST_OK(settings[_U("/Image/IDs[2]")].number() == number_type(234));
	CWT_TEST_OK(settings[_U("/Image/IDs[3]")].number() == number_type(38793));

	Vector<UniType> arr = settings.array(_U("/Image/IDs"));
	CWT_TEST_FAIL(arr.size() == 4);
	CWT_TEST_OK(arr[0].integer() == integer_type(116));
	CWT_TEST_OK(arr[1].number() == number_type(943));
	CWT_TEST_OK(arr[2].number() == number_type(234));
	CWT_TEST_OK(arr[3].number() == number_type(38793));
}

int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(19);

	test_settings_base();

	CWT_END_TESTS;
}

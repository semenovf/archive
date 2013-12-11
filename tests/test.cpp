#include <cwt/test.hpp>
#include <cwt/xml.hpp>
#include <iostream>

static const char * xml_data =
"<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>"
"﻿<orders>"
"   <order>"
"    <date>1/1/2011</date>"
"    <quantity>10</quantity>"
"    <description>Beverages</description>"
"  </order>"
"  <order>"
"    <date>2/2/2011</date>"
"    <quantity>15</quantity>"
"    <description>Condiments</description>"
"  </order>"
"  <order>"
"    <date>3/3/2011</date>"
"    <quantity>20</quantity>"
"    <description>Grains/Cereals</description>"
"  </order>"
"  <order>"
"    <date>4/6/2011</date>"
"    <quantity>30</quantity>"
"    <description>Beverages</description>"
"  </order>"
"  <order>"
"    <date>5/10/2011</date>"
"    <quantity>40</quantity>"
"    <description>Beverages</description>"
"  </order>"
"  <order>"
"    <date>5/12/2011</date>"
"    <quantity>50</quantity>"
"    <description>Grains/Cereals</description>"
"  </order>"
"</orders>";


static bool __accept_version(int major, int minor)
{
	TEST_OK(major == 1);
	TEST_OK(minor == 0);
	return true;
}

class handlers : public cwt::xml::handlers
{
	int _norders;
public:
	handlers() : cwt::xml::handlers(), _norders(0) {}
	int orderCount () const { return _norders; }

	virtual void startElement (const pfs::string & name, const pfs::map<pfs::string, pfs::string> &)
	{
		if (name == pfs::string("order"))
			++_norders;
	}
};

void test_basic ()
{
	handlers h;
	cwt::xml::reader xml(& h);

	xml.acceptVersion(__accept_version);
	TEST_OK(xml.parse(pfs::string::fromUtf8(xml_data)));
	if (xml.isError()) {
		std::cout << xml.errorString() << std::endl;
		TEST_FAIL(!xml.isError());
	}

	TEST_OK(h.orderCount() == 6);
}

int main(int argc, char *argv[])
{
    PFS_CHECK_SIZEOF_TYPES;
    PFS_UNUSED2(argc, argv);
    BEGIN_TESTS(4);

    test_basic();

    END_TESTS;
    return 0;
}

#include <cwt/test.hpp>
#include <cwt/xml/sax.hpp>
#include <iostream>

extern void test_dom();

static const char * xml_data =
"<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>\n"
"<!DOCTYPE mydoc PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\"\n"
" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\" [\n"
"  <!ELEMENT mydoc ANY>\n"
"  <!ENTITY % std       \"standard SGML\">\n"
"  <!ENTITY % signature \" &#x2014; &author;.\">\n"
"  <!ENTITY % question  \"Why couldn&#x2019;t I publish my books directly in &std;?\">\n"
"  <!ENTITY % author    \"William Shakespeare\">\n"
"]>\n"
"﻿<orders>"
"   <order>"
"    <date>1/1/2011</date>"
"    <quantity>10</quantity>"
"    <description>Beverages</description>"
"  </order>"
"<?target processing instruction?>"
"  <order>"
"    <date>2/2/2011</date>"
"    <quantity>15</quantity>"
"    <description>Condiments</description>"
"  </order>"
"<!--This is a comment text-->"
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
"<![CDATA["
" CDATA Text 0"
" CDATA Text 1"
" CDATA Text 2"
"]]>"
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
	bool _isCdata;
	pfs::string _cdata;
public:
	handlers() : cwt::xml::handlers(), _norders(0), _isCdata(false) {}
	int orderCount () const { return _norders; }

	virtual void startElement (const pfs::string & name, const pfs::map<pfs::string, pfs::string> &)
	{
		if (name == pfs::string("order"))
			++_norders;
	}

	virtual void characterData (const pfs::string & text)
	{
		if (_isCdata) {
			_cdata << text;
		} else {
			std::cout << "Character Data: [" << text << "]" << std::endl;
		}
	}

	virtual void processingInstruction (const pfs::string & target, const pfs::string & data)
	{
		std::cout << "Target: [" << target << "]; data=[" << data << "]" << std::endl;
		TEST_OK(target == "target");
		TEST_OK(data == "processing instruction");
	}

	virtual void comment (const pfs::string & text)
	{
		std::cout << "Comment: [" << text << "]" << std::endl;
		TEST_OK(text == "This is a comment text");
	}

	virtual void startCdataSection ()
	{
		_cdata.clear();
		_isCdata = true;
	}

	virtual void endCdataSection ()
	{
		std::cout<< "CDATA: ["  << _cdata << "]" << std::endl;
		_isCdata = false;
		TEST_OK(_cdata == " CDATA Text 0 CDATA Text 1 CDATA Text 2")
	}

	virtual void startDoctypeDecl (const pfs::string & doctypeName
			, const pfs::string & systemId
			, const pfs::string & publicId
			, bool has_internal_subset)
	{
		std::cout << "Doctype start: doctypeName=[" << doctypeName
				<< "]; systemId=[" << systemId
				<< "]; publicId=[" << publicId
				<< "]; has_internal_subset=[" << (has_internal_subset ? "true" : "false")
				<< "]" << std::endl;
		TEST_OK(doctypeName == "mydoc");
		TEST_OK(systemId == "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd");
		TEST_OK(publicId == "-//W3C//DTD XHTML 1.0 Transitional//EN");
		TEST_OK(has_internal_subset == true);
	}

	virtual void endDoctypeDecl ()
	{
		std::cout << "Doctype end" << std::endl;
	}

};

void test_basic ()
{
	handlers h;
	cwt::xml::reader xml(& h);

	xml.acceptVersion(__accept_version);
	TEST_OK(xml.parse(pfs::string::fromUtf8(xml_data)));
	if (xml.isError()) {
		std::cerr << xml;
	}

	TEST_OK(h.orderCount() == 6);
}

int main(int argc, char *argv[])
{
    PFS_CHECK_SIZEOF_TYPES;
    PFS_UNUSED2(argc, argv);
    BEGIN_TESTS(12);

if(0) {
    test_basic();
}
    test_dom();

    END_TESTS;
    return 0;
}

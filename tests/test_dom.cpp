/**
 * @file test_dom.cpp
 * @author wladt
 * @date Jan 21, 2014
 */

#include <cwt/test.hpp>
#include <cwt/xml/dom.hpp>
#include <iostream>

static const char * xml_source =
"<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>\n"
"<!DOCTYPE mydoc PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\"\n"
" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\" [\n"
"  <!ELEMENT mydoc ANY>\n"
"  <!ENTITY % std       \"standard SGML\">\n"
"  <!ENTITY % signature \" &#x2014; &author;.\">\n"
"  <!ENTITY % question  \"Why couldn&#x2019;t I publish my books directly in &std;?\">\n"
"  <!ENTITY % author    \"William Shakespeare\">\n"
"]>\n"
"﻿<orders>\n"
"   <order>\n"
"    <date>1/1/2011</date>\n"
"    <quantity>10</quantity>\n"
"    <description>Beverages</description>\n"
"  </order>\n"
"<?target processing instruction?>\n"
"  <order>\n"
"    <date>2/2/2011</date>\n"
"    <quantity>15</quantity>\n"
"    <description>Condiments</description>\n"
"  </order>\n"
"<!--This is a comment text-->\n"
"  <order>\n"
"    <date>3/3/2011</date>\n"
"    <quantity>20</quantity>\n"
"    <description>Grains/Cereals</description>\n"
"  </order>\n"
"  <order>\n"
"    <date>4/6/2011</date>\n"
"    <quantity>30</quantity>\n"
"    <description>Beverages</description>\n"
"  </order>\n"
"  <order>\n"
"    <date>5/10/2011</date>\n"
"    <quantity>40</quantity>\n"
"    <description>Beverages</description>\n"
"  </order>\n"
"  <order>\n"
"    <date>5/12/2011</date>\n"
"    <quantity>50</quantity>\n"
"    <description>Grains/Cereals</description>\n"
"  </order>\n"
"<![CDATA[\n"
" CDATA Text 0\n"
" CDATA Text 1\n"
" CDATA Text 2\n"
"]]>\n"
"</orders>";

void test_dom ()
{
	cwt::xml::dom xmlDom;
	cwt::dom::document xmlDoc = xmlDom.createDocument(_u8(xml_source));

	if (!xmlDom.isError())
		xmlDom.logErrors();

	TEST_FAIL(!xmlDoc.isNull());
	TEST_OK(xmlDoc.doctype().name() == "mydoc");
	TEST_OK(xmlDoc.doctype().systemId() == "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd");
	TEST_OK(xmlDoc.doctype().publicId() == "-//W3C//DTD XHTML 1.0 Transitional//EN");

	cwt::dom::nodelist list = xmlDoc.getElementsByTagName(_l1("orders"));
	TEST_OK(list.length() == 1);
	TEST_OK(list.item(0).nodeName() == "orders");

	cwt::dom::nodelist orders = xmlDoc.getElementsByTagName(_l1("order"));
	TEST_OK(orders.length() == 6);

	std::cout << xmlDoc << std::endl;
}


struct TraverseData
{
	int nodeCount;
	int balance;
	int indent;

	TraverseData() : nodeCount(0), balance(0), indent(0) {}
};

static void __on_start (const cwt::dom::node & n, void * userData)
{
	TraverseData * d = static_cast<TraverseData *>(userData);
	std::cout << std::string(d->indent * 4, ' ') << '<' <<  n.nodeName() << '>' << std::endl;
	++d->nodeCount;
	++d->balance;
	++d->indent;
}

static void __on_end (const cwt::dom::node & n, void * userData)
{
	TraverseData * d = static_cast<TraverseData *>(userData);
	--d->balance;
	--d->indent;
	std::cout << std::string(d->indent * 4, ' ') << "</" <<  n.nodeName() << '>' << std::endl;
}

void test_traverse ()
{
	cwt::xml::dom xmlDom;
	cwt::dom::document xmlDoc = xmlDom.createDocument(_u8(xml_source));

	if (!xmlDom.isError())
		xmlDom.logErrors();

	TraverseData userData;
	xmlDom.traverse(xmlDoc, __on_start, __on_end, & userData);

	TEST_OK(userData.nodeCount == 116);
	TEST_OK(userData.balance == 0);
	//std::cout << "userData.nodeCount = " << userData.nodeCount << std::endl;
}

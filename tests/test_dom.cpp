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
}

###NAME

`cwt-xml` - implements SAX parser and XML DOM builder.

###VERSION

Version 1.00a

###SYNOPSIS

Create DOM Document:

	#include <cwt/xml/dom.hpp>  
	...  
	cwt::xml::dom xmlDom;  
	cwt::dom::document xmlDoc = xmlDom.createDocument(_u8(xml_source));  
	  
	if (!xmlDom.isError()) // or xmlDoc.isNull()  
		xmlDom.logErrors();  
	
###AUTHOR

Fedor Semenov, < fedor.v.semenov at gmail.com >

###SUPPORT

No documentation prepared yet.  
This software is under development.

###LICENSE AND COPYRIGHT

Copyright 2013-2014 Fedor Semenov.

package MetaPage::Exception;
use Exception::Class (
    __PACKAGE__.'::XMLUnexpectedEvent' => {
        description=>'unexpected event'
        #return sprintf("unexpected XML parser event '%s' at line %s", $event, $xml_parser->current_line);
    },
    __PACKAGE__.'::XMLAttributeExpected' => { #expected attribute '%s' for '%s' at line %s
        description=>'attribute expected'
    },
    __PACKAGE__.'::XMLUnexpectedAttribute' => { # unexpected attribute for '%s'
        description=>'unexpected attribute'
    },
    __PACKAGE__.'::ParseError' => {
        description=>'parse error'
    }
);

1;

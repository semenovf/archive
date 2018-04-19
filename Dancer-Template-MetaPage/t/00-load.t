#!perl -T

use Test::More tests => 1;

BEGIN {
    use_ok( 'Dancer::Template::MetaPage' ) || print "Bail out!\n";
}

diag( "Testing Dancer::Template::MetaPage $Dancer::Template::MetaPage::VERSION, Perl $], $^X" );

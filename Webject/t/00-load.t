#!perl -T

use Test::More tests => 1;

BEGIN {
    use_ok( 'Webject' ) || print "Bail out!\n";
}

diag( "Testing Webject $Webject::VERSION, Perl $], $^X" );

#!perl -T

use Test::More tests => 1;

BEGIN {
    use_ok( 'Class::Accessor::Validated' ) || print "Bail out!\n";
}

diag( "Testing Class::Accessor::Validated $Class::Accessor::Validated::VERSION, Perl $], $^X" );

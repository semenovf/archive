#!perl -T

use Test::More tests => 1;

BEGIN {
    use_ok( 'Text::Simplify' ) || print "Bail out!\n";
}

diag( "Testing Text::Simplify $Text::Simplify::VERSION, Perl $], $^X" );

#!perl -T

use Test::More tests => 1;

BEGIN {
    use_ok( 'DBI::DDI' ) || print "Bail out!\n";
}

diag( "Testing DBI::DDI $DBI::DDI::VERSION, Perl $], $^X" );

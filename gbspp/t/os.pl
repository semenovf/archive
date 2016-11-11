#!/usr/bin/perl -w
use strict;

use lib qw(../../);
use Test::Simple tests => 1;
use strict;

package test::os;
use gbspp::os;

sub run
{
    print "Variable \$^O => '$^O'\n";
#    ok(gbspp::type::bool::can_assign('true') , q(Boolean can be assigned from 'true'));
}

package main;

test::os::run();

0;

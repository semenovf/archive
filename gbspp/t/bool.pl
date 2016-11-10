#!/usr/bin/perl -w
use strict;

use lib qw(../../);
use Test::Simple tests => 29;
use strict;

package test::dummy_class;

sub new {
    return bless {}, __PACKAGE__;
}

package test::bool_can_assign;
use Test::Simple;
use gbspp::type::bool;

sub run
{
    ok(gbspp::type::bool::can_assign('true') , q(Boolean can be assigned from 'true'));
    ok(gbspp::type::bool::can_assign('TRUE') , q(Boolean can be assigned from 'TRUE'));
    ok(gbspp::type::bool::can_assign('false'), q(Boolean can be assigned from 'false'));
    ok(gbspp::type::bool::can_assign('FALSE'), q(Boolean can be assigned from 'FALSE'));
    ok(gbspp::type::bool::can_assign(1)      , q(Boolean can be assigned from 1));
    ok(gbspp::type::bool::can_assign(-1)     , q(Boolean can be assigned from -1));
    ok(gbspp::type::bool::can_assign(0)      , q(Boolean can be assigned from 0));
    
    my $true  = gbspp::type::bool->new(1);
    my $false = gbspp::type::bool->new(0);
    
    ok(gbspp::type::bool::can_assign($true)  , q(Boolean can be assigned from other 'true' Boolean object));
    ok(gbspp::type::bool::can_assign($false) , q(Boolean can be assigned from other 'false' Boolean object));
    
    ok(! gbspp::type::bool::can_assign('True') , q(Boolean can not be assigned from 'True'));
    ok(! gbspp::type::bool::can_assign('TruE') , q(Boolean can not be assigned from 'TruE'));
    ok(! gbspp::type::bool::can_assign('False'), q(Boolean can not be assigned from 'False'));
    ok(! gbspp::type::bool::can_assign('FalsE'), q(Boolean can not be assigned from 'FalsE'));
    
    ok(! gbspp::type::bool::can_assign(()), q(Boolean can not be assigned from empty array/hash));
    ok(! gbspp::type::bool::can_assign([]), q(Boolean can not be assigned from empty array reference));
    ok(! gbspp::type::bool::can_assign({}), q(Boolean can not be assigned from empty hash reference));
    
    my $hashref  = { 1 => 2 };
    my $arrayref = [ 1, 2 ];
    ok(! gbspp::type::bool::can_assign($arrayref), q(Boolean can not be assigned from array reference));
    ok(! gbspp::type::bool::can_assign($hashref), q(Boolean can not be assigned from hash reference));
    
    my $dummy_obj = test::dummy_class->new;
    ok(! gbspp::type::bool::can_assign($dummy_obj), q(Boolean can not be assigned from any object besides 'bool'));
}

package test::bool_assign;
use Test::Simple;
use gbspp::type::bool;

sub run
{
    my $b = gbspp::type::bool->new(0);
    ok($b->value == 0, q(Boolean is initialized with '0'));

    $b->assign('true');
    ok($b->value == 1, q(Boolean is assigned from 'true'));

    $b->assign('TRUE');
    ok($b->value == 1, q(Boolean is assigned from 'TRUE'));

    $b->assign(1);
    ok($b->value == 1, q(Boolean is assigned from 1));
    
    $b->assign(-1);
    ok($b->value == 1, q(Boolean is assigned from -1));

    $b->assign(0);
    ok($b->value == 0, q(Boolean is assigned from 0));

    $b->assign('false');
    ok($b->value == 0, q(Boolean is assigned from 'false'));

    $b->assign('FALSE');
    ok($b->value == 0, q(Boolean is assigned from 'FALSE'));
    
    my $true  = gbspp::type::bool->new(1);
    my $false = gbspp::type::bool->new(0);
    
    $b->assign($true);
    ok($b->value == 1, q(Boolean is assigned from other 'true' Boolean object));
    
    $b->assign($false);
    ok($b->value == 0, q(Boolean is assigned from other 'false' Boolean object));
}

package main;

test::bool_can_assign::run();
test::bool_assign::run();

0;

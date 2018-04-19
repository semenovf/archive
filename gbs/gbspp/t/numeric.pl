#!/usr/bin/perl -w
use lib qw(../../);
use Test::Simple tests => 13;
use strict;

package test::dummy_class;

sub new
{
    return bless {}, __PACKAGE__;
}

package test::numeric_can_assign;
use Test::Simple;
use gbspp::type::numeric;

sub run
{
    ok(gbspp::type::numeric->can_assign(123)      , q(Numeric can be assigned from number));
    ok(! gbspp::type::numeric->can_assign('string') , q(Numeric can not be assigned from 'string'));
        
    my $numeric  = gbspp::type::numeric->new(123);
    ok(gbspp::type::numeric->can_assign($numeric)  , q(Numeric can be assigned from other Numeric object));

    ok(! gbspp::type::numeric->can_assign(()), q(Numeric can not be assigned from empty array/hash));
    ok(! gbspp::type::numeric->can_assign([]), q(numeric can not be assigned from empty array reference));
    ok(! gbspp::type::numeric->can_assign({}), q(numeric can not be assigned from empty hash reference));
    
    my $hashref  = { 1 => 2 };
    my $arrayref = [ 1, 2 ];
    ok(! gbspp::type::numeric->can_assign($arrayref), q(Numeric can not be assigned from array reference));
    ok(! gbspp::type::numeric->can_assign($hashref), q(Numeric can not be assigned from hash reference));
    
    my $dummy_obj = test::dummy_class->new;
    ok(! gbspp::type::numeric->can_assign($dummy_obj), q(Numeric can not be assigned from any object besides Numeric));
}

package test::numeric_assign;
use Test::Simple;
use gbspp::type::numeric;

sub run
{
    my $n = gbspp::type::numeric->new(0);
    ok($n->value == 0, q(Numeric is initialized with '0'));
    
    $n->assign(123);
    ok($n->value == 123, q(Numeric is assigned from number 123));
    
    $n->assign(-1);
    ok($n->value == -1, q(Numeric is assigned from number -1));
    
    my $numeric = gbspp::type::numeric->new(3.14159);

    $n->assign($numeric);
    ok($n->value == $numeric->value, q(Numeric is assigned from other Numeric object));
}

package main;

test::numeric_can_assign::run();
test::numeric_assign::run();

0;

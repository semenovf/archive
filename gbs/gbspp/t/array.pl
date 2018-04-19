#!/usr/bin/perl -w
use lib qw(../../);
use Test::Simple tests => 29;
use strict;

package test::dummy_class;

sub new
{
    return bless {}, __PACKAGE__;
}

package test::array;
use Test::Simple;
use gbspp::type::array;

sub run
{
    my $a1 = gbspp::type::array->new('bool');
    #my $a2 = gbspp::type::array->new('numeric');
    #my $a3 = gbspp::type::array->new('string');
    
    ok($a1->size == 0);
    
    $a1->push_back(1);
    $a1->push_back(0);
    $a1->push_back(1);
    
    ok($a1->size == 3);
    ok($a1->at(0)->equals(1));
    ok($a1->at(1)->equals(0));
    ok($a1->at(2)->equals(1));
    
    ok($a1->front()->equals(1));
    ok($a1->back()->equals(1));
    
    #
    #my $hashref  = { 1 => 2 };
    #my $arrayref = [ 1, 2 ];
    #ok(! gbspp::type::bool->can_assign($arrayref), q(Boolean can not be assigned from array reference));
    #ok(! gbspp::type::bool->can_assign($hashref), q(Boolean can not be assigned from hash reference));
    #
    #my $dummy_obj = test::dummy_class->new;
    #ok(! gbspp::type::bool->can_assign($dummy_obj), q(Boolean can not be assigned from any object besides 'bool'));
}

package main;

test::array::run();

0;


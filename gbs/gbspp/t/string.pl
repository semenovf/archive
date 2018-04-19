#!/usr/bin/perl -w
use lib qw(../../);
use Test::Simple tests => 14;
use strict;

package test::dummy_class;

sub new
{
    return bless {}, __PACKAGE__;
}

package test::string_can_assign;
use Test::Simple;
use gbspp::type::string;

sub run
{
    ok(gbspp::type::string->can_assign('string') , q(String can be assigned from 'string'));
    ok(gbspp::type::string->can_assign(123)      , q(String can be assigned from number));
    
    my $string  = gbspp::type::string->new('hello');
    ok(gbspp::type::string->can_assign($string)  , q(String can be assigned from other String object));

    ok(! gbspp::type::string->can_assign(()), q(String can not be assigned from empty array/hash));
    ok(! gbspp::type::string->can_assign([]), q(String can not be assigned from empty array reference));
    ok(! gbspp::type::string->can_assign({}), q(String can not be assigned from empty hash reference));
    
    my $hashref  = { 1 => 2 };
    my $arrayref = [ 1, 2 ];
    ok(! gbspp::type::string->can_assign($arrayref), q(String can not be assigned from array reference));
    ok(! gbspp::type::string->can_assign($hashref), q(String can not be assigned from hash reference));
    
    my $dummy_obj = test::dummy_class->new;
    ok(! gbspp::type::string->can_assign($dummy_obj), q(String can not be assigned from any object besides 'string'));
}

package test::string_assign;
use Test::Simple;
use gbspp::type::string;

sub run
{
    my $s = gbspp::type::string->new(0);
    ok($s->value eq '0', q(String is initialized with '0'));
    
    $s->assign('string');
    ok($s->value eq 'string', q(String is assigned from 'string'));

    $s->assign(123);
    ok($s->value eq '123', q(String is assigned from number 123));
    
    $s->assign(-1);
    ok($s->value eq '-1', q(String is assigned from number -1));
    
    my $string = gbspp::type::string->new('hello');

    $s->assign($string);
    ok($s->value eq $string->value, q(String is assigned from other String object));
}

package main;

test::string_can_assign::run();
test::string_assign::run();

0;

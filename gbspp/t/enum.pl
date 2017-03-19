#!/usr/bin/perl -w
use lib qw(../);
use lib qw(../../);
use lib qw(../../../);
use Test::Simple tests => 14;
use strict;

package test::dummy_class;

sub new
{
    return bless {}, __PACKAGE__;
}

package test::enum_can_assign;
use Test::Simple;
use gbspp::type::enum;
use gbspp::type::string;

sub run
{
    my $string_enum = gbspp::type::enum->new('string', ['One', 'Two', 'Three']);
    my $numeric_enum = gbspp::type::enum->new('numeric', [1, 2, 3]);
    
    ok($string_enum->can_assign('One')  , q(String enum can be assigned from 'One'));
    ok($string_enum->can_assign('Two')  , q(String enum can be assigned from 'Two'));
    ok($string_enum->can_assign('Three'), q(String enum can be assigned from 'Three'));
    ok(!$string_enum->can_assign('Four'), q(String enum can not be assigned from 'Four'));
    
    ok($numeric_enum->can_assign(1)  , q(Numeric enum can be assigned from 1));
    ok($numeric_enum->can_assign(2)  , q(Numeric enum can be assigned from 2));
    ok($numeric_enum->can_assign(3)  , q(Numeric enum can be assigned from 3));
    ok(!$numeric_enum->can_assign(0) , q(Numeric enum can not be assigned from 0));
    ok(!$numeric_enum->can_assign(4) , q(Numeric enum can not be assigned from 4));
    ok(!$numeric_enum->can_assign(-1), q(Numeric enum can not be assigned from -1));
    
    my $string_enum1 = gbspp::type::enum->new('string'
        , [
              gbspp::type::string->new('One')
            , gbspp::type::string->new('Two')
            , gbspp::type::string->new('Three')
        ]);
    
    ok($string_enum->can_assign($string_enum1), q(String enum can be assigned string object));
    
    ok($string_enum1->can_assign($string_enum), q(String enum can be assigned string object));
    ok($string_enum1->can_assign('One'), q(String enum can be assigned to 'One'));
    ok($string_enum1->can_assign('Two'), q(String enum can be assigned to 'Two'));
    ok($string_enum1->can_assign('Three'), q(String enum can be assigned to 'Three'));
    ok(!$string_enum1->can_assign('Four'), q(String enum can not be assigned to 'Four'));
    
    ok(! $string_enum1->can_assign(()), q(String enum can not be assigned from empty array/hash));
    ok(! $string_enum1->can_assign([]), q(String enum can not be assigned from empty array reference));
    ok(! $string_enum1->can_assign({}), q(String enum can not be assigned from empty hash reference));
    
    my $hashref  = { 1 => 2 };
    my $arrayref = [ 1, 2 ];
    ok(! $string_enum1->can_assign($arrayref), q(String enum can not be assigned from array reference));
    ok(! $string_enum1->can_assign($hashref), q(String enum can not be assigned from hash reference));
    
    my $dummy_obj = test::dummy_class->new;
    ok(! $string_enum1->can_assign($dummy_obj), q(String enum can not be assigned from dummy object));
}

package test::enum_assign;
use Test::Simple;
use gbspp::type::enum;

sub run
{
    my $e = gbspp::type::enum->new('string', ['One', 'Two', 'Three']);
    ok($e->value eq 'One', q(String enum is initialized with 'One' - first element in initializer list));
    
    $e->assign('Two');
    ok($e->value eq 'Two', q(String enum is assigned to 'Two'));

    $e->assign('Three');
    ok($e->value eq 'Three', q(String enum is assigned to 'Three'));

    my $e1 = gbspp::type::enum->new('string'
        , [
              gbspp::type::string->new('One')
            , gbspp::type::string->new('Two')
            , gbspp::type::string->new('Three')
        ]);
    
    $e->assign($e1);
    ok($e->equals($e1), q(String enum is assigned from other String enum object));
}

package main;

test::enum_can_assign::run();
test::enum_assign::run();

0;

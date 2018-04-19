#!perl
use strict;
use Test::More;

unless (eval {require Sub::Name}) {
    plan skip_all => "Sub::Name is not installed";
    exit 0;
}

plan tests => 5;

require_ok("Class::Accessor::Validated");

package Foo;
our @ISA = qw(Class::Accessor::Validated);
sub get {
    my ($self, $key) = @_;
    my @c = caller(1);
    main::is $c[3], "Foo::$key", "correct name for Foo sub $key";
    return $self->SUPER::get($key);
}
__PACKAGE__->mk_accessors(foo=>sub{1});

package main;
use Data::Dumper;
local $SIG{__WARN__} = sub {};
my $foo = Foo->new( {foo => 12345} );
is $foo->foo, undef, "arguments passed to constructor are ignored";
$foo->foo(12345);
is $foo->foo, 12345, "get foo";

#!perl
use strict;
use Test::More tests => 3;

require_ok("Class::Accessor::Validated");

@Foo::ISA = qw(Class::Accessor::Validated);
Foo->mk_accessors(foo=>sub{1});

my $test = Foo->new({ foo => 49 });

is $test->get('foo'), 49, "get initial foo";
$test->set('foo', 42);
is $test->get('foo'), 42, "get new foo";

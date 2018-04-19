#!perl
use strict;
use Test::More tests => 12;

for my $class (qw(Class::Accessor::Validated)) {
    require_ok($class);
    my $silly = "Silly::$class";
    {
        no strict 'refs';
        @{"${silly}::ISA"} = ($class);
        $silly->follow_best_practice;
        $silly->mk_accessors(foo=>sub{1});
        $silly->mk_ro_accessors(roro=>sub{1});
        $silly->mk_wo_accessors(wowo=>sub{1});
    }

    for my $f (qw/foo roro /) {
        ok $silly->can("get_$f"), "'get_$f' method exists";
    }

    for my $f (qw/foo wowo/) {
        ok $silly->can("set_$f"), "'set_$f' method exists";
    }

    for my $f (qw/foo roro wowo set_roro get_wowo/) {
        ok !$silly->can($f), "no '$f' method";
    }

    my $test = $silly->new({
            foo => "bar",
            roro => "boat",
            wowo => "huh",
        });

    is($test->get_foo, "bar", "initial foo");
    $test->set_foo("stuff");
    is($test->get_foo, "stuff", "new foo");
}

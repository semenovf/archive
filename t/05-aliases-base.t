#!perl
use strict;
use Test::More tests => 12;

for my $class (qw(Class::Accessor::Validated)) {
    require_ok($class);
    my $silly = "Silly::$class";
    {
        no strict 'refs';
        @{"${silly}::ISA"} = ($class);
        *{"${silly}::accessor_name_for"} = sub { "read_$_[1]" };
        *{"${silly}::mutator_name_for"} = sub { "write_$_[1]" };
        $silly->mk_accessors(foo=>sub {1});
        $silly->mk_ro_accessors(roro=>sub {1});
        $silly->mk_wo_accessors(wowo=>sub {1});
    }

    for my $f (qw/foo roro/) {
        ok $silly->can("read_$f"), "'read_$f' method exists";
    }

    for my $f (qw/foo wowo/) {
        ok $silly->can("write_$f"), "'write_$f' method exists";
    }

    for my $f (qw/foo roro wowo write_roro read_wowo/) {
        ok !$silly->can($f), "no '$f' method";
    }

    my $test = $silly->new({
            foo => "bar",
            roro => "boat",
            wowo => "huh",
        });

    is($test->read_foo, "bar", "initial foo");
    $test->write_foo("stuff");
    is($test->read_foo, "stuff", "new foo");
}

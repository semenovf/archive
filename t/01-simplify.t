use strict;
use warnings;

use Test::More tests => 10;

BEGIN {
    use_ok('Text::Simplify');
}

my $text = my $orig = "\t\n  foo\t   bar \t\n";
is(simplify($text), 'foo bar', 'simplify in scalar context');
is($text, $orig, ".. didn't affect original");

simplify($text);
is($text, 'foo bar', 'simplify in place changes original');

{
    local $_ = $orig;
    my $simplified = simplify;
    is($simplified, 'foo bar', '$scalar = simplify() works on $_');
    is($_,     , $orig, ".. didn't affect original");

    simplify;
    is($_,     , 'foo bar', "simplify() alters \$_")
}

my @before = (
    "   foo   foo   ",
    " \n bar \n bar \n ",
    " \n\t baz \n \t baz \n\t "
);
my @expected = qw( 'foo foo' 'bar bar' 'baz baz' );
my @simplified = simplify @before;
is_deeply(\@simplified, \@expected, 'simplify on a list in list content');
simplify @before;
is_deeply(\@before, \@expected,  'simplify on a list in place');

my $expected = "@expected";
my $simplified = simplify @before;
is($simplified, $expected, 'simplify on a list in scalar context');

__END__

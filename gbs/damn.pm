package gbs::damn;
use Carp;
use strict;
use warnings;

require Exporter;
our @ISA = qw(Exporter);
our @EXPORT = qw(kick damn);

sub kick
{
    my $package = (caller)[0];
    
    # see https://github.com/miyagawa/Data-Section-Simple/blob/master/lib/Data/Section/Simple.pm
    my $d = do { no strict 'refs'; \*{$package."::DATA"} };
    local $/;
    my $out = <$d>;
    my @s = caller(1);
    warn 'ERROR: ' , join(' ', @_), " at " , $s[1] . ':' . $s[2] , "\n";
    warn $out;
}

1;

package gbspp::sub;
use Carp;
use strict;
use warnings;

require Exporter;
our @ISA = qw(Exporter);
our @EXPORT_OK = qw(required);

sub required
{
    my $a = shift or die;
    
    unless (ref($a) eq 'ARRAY') {
        my ($p, $f, $l) = caller;
        croak "$f:$l: required parameter expected";
    }
    
    return shift @$a;
}

1;

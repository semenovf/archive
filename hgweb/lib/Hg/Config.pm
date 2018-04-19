package Hg::Config;
use strict;
use warnings;

use Exporter qw(import);
our @EXPORT = qw(config %config);
my %config = ();
sub config
{
    die unless @_;
    
    my $class = shift;
    $class = ref $class if ref $_[0];
    my %args = @_;
    my $class_config = $config{$class} || {};

    if (@_ > 0) {
        @$class_config{keys %args} = values %args;
        $config{$class} = $class_config;
    }
    
    return $class_config;
}

1;
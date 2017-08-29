package gbs::includedirs;
use gbs::damn;
use gbs::context;
use strict;
use warnings;

require Exporter;
our @ISA = qw(Exporter);
our @EXPORT = qw(includedirs);

sub includedirs
{
    kick "includedirs" if scalar(@_) != 1;
    #context()->includedirs(@_);
}

1;

__DATA__
usage:
    includedirs "DIR"
    includedirs ["DIR1", "DIR2" , ...]

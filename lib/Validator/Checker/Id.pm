package Validator::Checker::Id;
use 5.006;
use strict;
use warnings;

our (@ISA, @EXPORT_OK);
BEGIN {
    require Exporter;
    @ISA = qw(Exporter);
    @EXPORT_OK = qw(Id);
}

sub Id     { [\&_id, 'Id', 'simple identificator' ] }

sub _id
{
    return $_[0] =~ /^[a-zA-Z][_a-zA-Z0-9]*$/;
}
1;

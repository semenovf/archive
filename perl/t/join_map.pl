#!/usr/bin/perl -w

my @arr = ( 'one', 'two', 'three' );
my $hashref = {
    'array' => \@arr
};

print join(',', map { '`'.$_.'`'} @{$hashref->{'array'}} ), "\n";

0;

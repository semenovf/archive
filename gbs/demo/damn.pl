#!/usr/bin/perl -w
use lib qw(../../);
use gbs::includedirs;
use strict;

sub foo
{
    includedirs();
}

foo();

0;


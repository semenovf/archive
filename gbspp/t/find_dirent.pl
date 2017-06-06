#!/usr/bin/perl -w
use lib qw(../../);
use Test::Simple tests => 2;
use strict;

package test0;
use Test::Simple;
use gbspp::find_dirent;
#use Data::Dumper;
use strict;

sub run
{
#    print Dumper($cli), "\n";
    
    ok(! find_dirent('no-such-direntry-found'));
    ok(find_dirent(dirent_name=>'usr', dirs=>['/']));
    
}

package main;
test0::run;

0;

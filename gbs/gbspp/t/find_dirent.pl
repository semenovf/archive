#!/usr/bin/perl -w
use lib qw(../../);
use Test::Simple tests => 6;
use strict;

package test0;
use Test::Simple;
use gbspp::find_dirent;
use strict;

sub run
{
    ok(! find_dirent('no-such-direntry-found'));
    ok(find_dirent(dirent_name=>'usr', dirs=>['/']));
    
    ok(find_file('ls'));
    ok(find_program('ls'));
    ok(find_dir(dirent_name=>'bin', dirs=>['/usr']));
    
    my $dir = find_dir(dirent_name=>'bin'
                , no_default_path=>1
                , dirs=>['/usr/unknown_dir1', '/usr/unknown_dir1', '/usr']);
    ok($dir eq '/usr/bin');
}

package main;
test0::run;

0;

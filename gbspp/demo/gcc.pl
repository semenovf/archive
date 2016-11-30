#!/usr/bin/perl -w
use lib qw(../../);
use lib qw(../../gbspp);
use File::Find;
use gbspp::term;
use strict;

my $c_lang_source_pattern = '\.[cC]$';

my $base_dir = '.';

if ($#ARGV >= 0) {
   $base_dir = $ARGV[0]; 
}

File::Find::find({
    wanted => sub {
        if (-d $_) {
            return;
        }
        
        if (/$c_lang_source_pattern/) {
            printf("%s: %s\n", $File::Find::dir, $_);
        }
    }
    , no_chdir => 0
    , bydepth => 0}
    , $base_dir
);

#gbspp::term::set_info_color('green');
#gbspp::term::set_debug_color('bright_black');
#gbspp::term::set_warn_color('magenta');
#gbspp::term::set_error_color('red');
#
#my @result = gbspp::term::run_command('gcc', '-v');
#print "STDOUT: {", gbspp::term::info($result[0]), "}\n";
#print "STDERR: {", gbspp::term::error($result[1]), "}\n";
#print "STATUS: {", gbspp::trem::info($result[2]), "}\n";

0;

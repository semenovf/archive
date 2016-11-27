#!/usr/bin/perl -w
use lib qw(../../);
use lib qw(../../gbspp);
use File::Find;
use gbspp::shell;
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

#gbspp::shell::set_info_color('green');
#gbspp::shell::set_debug_color('bright_black');
#gbspp::shell::set_warn_color('magenta');
#gbspp::shell::set_error_color('red');
#
#my @result = gbspp::shell::run('gcc', '-v');
#print "STDOUT: {", gbspp::shell::info($result[0]), "}\n";
#print "STDERR: {", gbspp::shell::error($result[1]), "}\n";
#print "STATUS: {", gbspp::shell::info($result[2]), "}\n";

0;

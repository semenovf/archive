#!/usr/bin/perl -w
use lib qw(../../);
use lib qw(../../gbspp);
use gbspp::shell;
use strict;

gbspp::shell::set_error_color('red');
gbspp::shell::set_info_color('green');

my @result = gbspp::shell::run('gcc', '-v');
print "STDOUT: {", gbspp::shell::info($result[0]), "}\n";
print "STDERR: {", gbspp::shell::error($result[1]), "}\n";
print "STATUS: {", gbspp::shell::info($result[2]), "}\n";

0;

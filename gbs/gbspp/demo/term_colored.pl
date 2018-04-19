#!/usr/bin/perl -w
use lib qw(../../);
use lib qw(../../gbspp);
use gbspp::term;
use strict;

gbspp::term::set_info_color('green');
gbspp::term::set_debug_color('bright_black');
gbspp::term::set_warn_color('magenta');
gbspp::term::set_error_color('red');

print gbspp::term::info("Info message"), "\n";
print gbspp::term::debug("Debug message"), "\n";
print gbspp::term::warn("Warn message"), "\n";
print gbspp::term::error("Error message"), "\n";

0;

#!/usr/bin/perl -w
use lib qw(../../);
use lib qw(../../gbspp);
use gbspp::shell;
use strict;

gbspp::shell::set_info_color('green');
gbspp::shell::set_debug_color('bright_black');
gbspp::shell::set_warn_color('magenta');
gbspp::shell::set_error_color('red');

print gbspp::shell::info("Info message"), "\n";
print gbspp::shell::debug("Debug message"), "\n";
print gbspp::shell::warn("Warn message"), "\n";
print gbspp::shell::error("Error message"), "\n";

0;

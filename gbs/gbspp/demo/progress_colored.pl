#!/usr/bin/perl -w
use lib qw(../../);
use lib qw(../../gbspp);
use Time::HiRes qw(usleep);
use gbspp::term;
use strict;

my $DONE_CHAR  = '#';
my $REST_CHAR  = ' ';
my $BEGIN_CHAR = '[';
my $END_CHAR   = ']';

sub progress_bar
{
    my ($percents, $columns) = @_;
    die unless defined $percents;
    
    $columns = gbspp::term::term_width unless defined $columns;
    $columns = 80 unless defined $columns;
    
    my $length = 100;
    
    if ($columns < 107) {
        $length = $columns - 7;
    }

    my $done_length = int($length * $percents/100);
    my $rest_length = $length - $done_length;
        
    my $done = $DONE_CHAR x $done_length;
    my $rest = $REST_CHAR x $rest_length;
    my $text = sprintf("%3d%%", $percents);
    
    print gbspp::term::info(
          $text
        . ' '
        . $BEGIN_CHAR
        . $done
        . $rest
        . $END_CHAR), "\r";
}

$| = 1; # autoflush

gbspp::term::set_info_color('green');
my ($rows, $cols) = gbspp::term::term_size;

for (1..100) {
    progress_bar($_, $cols);
    usleep(10000);
}

print "\n";

0;

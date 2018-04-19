#!/usr/bin/perl -w
use threads;

#binmode STDOUT, ":encoding(utf8)";

sub run_threads
{
    for my $i (1..3) {
      threads->create(\&get_now, $i);
    }

    foreach ( threads->list ) {
        $_->join();
    }
}


sub get_now
{
    my $num = shift;
    print "thread ", $num, " => ", time(), "\n";
    sleep 1;
}


run_threads();

0;

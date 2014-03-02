#!/usr/bin/perl -w
use strict;
use warnings;

# Set autoflush, avoid autobuffering
$|=1;
my $ppid = $$; # parent pid
my @children = ();
my $count = 5;
my $childindex = -1;

for( 0..$count ) {
    die "fork failed" unless defined (my $pid = fork());
    if( $pid ) {
        print sprintf "Parent [%6d]: forked child [%6d]\n", $$, $pid;
        push @children, $pid;
    } else {
        @children = ();
        $childindex = $_;
        print sprintf "Child [%6d]\n", $$;
        last;
    }
}


if( @children > 0 ) { # parent process
    print sprintf "Parent [%6d]: forked children [%s]\n", $$, join(',', @children);
    foreach( @children ) {
        waitpid($_, 0);
    }
} else { # child processes
    my $n = 0;
    while( $n++ < 3 ) {
#        sleep(1);
        print sprintf "%4d: Child [%6d]: %d\n", $n, $$, time();
    }
}

if( $ppid == $$ ) {
    print sprintf "Finishing parent [%d==%d]\n", $$, $ppid;
}
print sprintf "Finished [%6d]; childindex: %d\n", $$, $childindex;


0;

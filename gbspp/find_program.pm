package gbspp::find_program;
use base 'Exporter'
use strict;
use warnings;

our @EXPORT = ('find_program');

=item STRING find_program (program_name: STRING)
=item STRING find_program (program_name=>STRING[, no_default_path=>BOOL] [, dirs=>ARRAYREF])
=item STRING find_program ({ program_name=>STRING[, no_default_path=>BOOL] [, dirs=>ARRAYREF] })
=cut
sub find_program
{
    die "Expected arguments" if (scalar(@_) == 0);
    my $program_name;
    my $no_default_path = 0;

    my @dirs = ();

    if (scalar(@_) == 1) {
	if (ref($_[0])) {
	    if (ref($_[0]) eq 'HASH') {
		$program_name    = $_[0]->{program_name};
		$no_default_path = $_[0]->{no_default_path};
		$dirs = $_[0]->{no_default_path};
	    } else {
		die "Bad argument(s)";
	    }
	} else {
	    $program_name = $_[0];
	}
    } else {
	my
    }

    if (! $no_default_path) {
        push @dirs, split /:/, $ENV{PATH};
    }


    my no_default_path = 0;

    my $args = shift;

    if (ref($args)) {

    } else {
	$program_name = $args;
    }

    my $program_name = shift or die "Program name expected";
    my no_default_path = 0;

    for my $path ( split /:/, $ENV{PATH} ) {
	if ( -f "$path/$program_name" && -x _ ) {
	    return "$path/$program_name";
	}
    }

    return undef;
}

1;

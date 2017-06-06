package gbspp::find_dirent;
use v5.14; # for given-when
use base 'Exporter';
use Carp;
use strict;
use warnings;

our @EXPORT = qw(find_file find_dir find_program find_dirent);

=item STRING find_direntry (<dirent_name>)
=item STRING find_direntry (dirent_type=>CHAR, dirent_name=>STRING[, no_default_path=>BOOL] [, dirs=>ARRAYREF])
=item STRING find_direntry ({ dirent_type=>CHAR, dirent_name=>STRING[, no_default_path=>BOOL] [, dirs=>ARRAYREF] })
=cut
sub find_dirent
{
    croak "Expected arguments" if (scalar(@_) == 0);
    my ($dirent_type, $dirent_name, $no_default_path);

    my @dirs = ();

    # First case
    if (scalar(@_) == 1 and !ref($_[0])) {
	$dirent_name = $_[0];
    } elsif ((scalar(@_) == 1 and ref($_[0]) eq 'HASH') # Second 
	     or scalar(@_) > 1) {                       # and third cases

	my $args;
	
	if (scalar(@_) == 1) {
	    $args = $_[0];
	} else {
	    %_ = @_;
	    $args = \%_;
	}
	
	($dirent_type, $dirent_name, $no_default_path) =
	    ($args->{dirent_type}, $args->{dirent_name}, $args->{no_default_path});
	    
	if (defined($args->{dirs}) and ref($args->{dirs}) eq 'ARRAY') {
	    push @dirs, @{$args->{dirs}};
	} else {
	    croak "`dirs` must be an array reference";
	}
    } else {
	croak "Bad argument(s)";
    }
    
    if (! $no_default_path) {
        push @dirs, split /:/, $ENV{PATH};
    }

    croak 'Direntry name must be specified' unless $dirent_name;
    $dirent_type = 'e' unless defined $dirent_type;
    $no_default_path = 0 unless defined $no_default_path;
    
    for my $dir ( @dirs ) {
	my $path = "$dir/$dirent_name";
	
	next unless (-e $path);
	
	SWITCH: {
	    if ($dirent_type eq 'e') { return $path if -e $path; last SWITCH; }
	    if ($dirent_type eq 'f') { return $path if -f $path; last SWITCH; }
	    if ($dirent_type eq 'd') { return $path if -d $path; last SWITCH; }
	    if ($dirent_type eq 'x') { return $path if -x $path; last SWITCH; }
	    croak "Bad dirent type";
	}
    }

    return undef;
}

=item STRING find_file (dirent_name: STRING)
=item STRING find_file (dirent_name=>STRING[, no_default_path=>BOOL] [, dirs=>ARRAYREF])
=item STRING ffind_file ({dirent_name=>STRING[, no_default_path=>BOOL] [, dirs=>ARRAYREF] })
=cut
sub find_file
{
}

=item STRING find_dir (dirent_name: STRING)
=item STRING find_dir (dirent_name=>STRING[, no_default_path=>BOOL] [, dirs=>ARRAYREF])
=item STRING ffind_dir ({dirent_name=>STRING[, no_default_path=>BOOL] [, dirs=>ARRAYREF] })
=cut
sub find_dir
{
}

=item STRING find_program (dirent_name: STRING)
=item STRING find_program (dirent_name=>STRING[, no_default_path=>BOOL] [, dirs=>ARRAYREF])
=item STRING ffind_program ({dirent_name=>STRING[, no_default_path=>BOOL] [, dirs=>ARRAYREF] })
=cut
sub find_program
{
    
}

1;

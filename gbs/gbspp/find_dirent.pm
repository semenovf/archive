package gbspp::find_dirent;
use v5.14; # for given-when
use base 'Exporter';
use Carp;
use strict;
use warnings;


=head1 find_dirent
=cut

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

	local $_;
	
	if (scalar(@_) == 1) {
	    $_ = $_[0];
	} else {
	    %_ = @_;
	    $_ = \%_;
	}
	
	($dirent_type, $dirent_name, $no_default_path) =
	    ($_->{dirent_type}, $_->{dirent_name}, $_->{no_default_path});
	
	if (defined($_->{dirs})) {
	    if (ref($_->{dirs}) eq 'ARRAY') {
		push @dirs, @{$_->{dirs}};
	    } else {
		croak "`dirs` must be an array reference";
	    }
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
	    croak "Bad directory entry type";
	}
    }

    return undef;
}

sub __find_dirent
{
    croak "Expected arguments" if (scalar(@_) == 0);
    my $dirent_type = shift;
    
    # First case
    return find_dirent({dirent_type=>$dirent_type, dirent_name=>$_[0]}) if (scalar(@_) == 1 and !ref($_[0]));

    # Second case    
    return find_dirent(@_, dirent_type=>$dirent_type) if scalar(@_) > 1;
    
    # Third case
    if ((scalar(@_) == 1 and ref($_[0]) eq 'HASH')) {
	$_[0]->{dirent_type} =$dirent_type;
	return find_dirent($_[0]);
    }    
}


=item STRING find_file (dirent_name: STRING)
=item STRING find_file (dirent_name=>STRING[, no_default_path=>BOOL] [, dirs=>ARRAYREF])
=item STRING ffind_file ({dirent_name=>STRING[, no_default_path=>BOOL] [, dirs=>ARRAYREF] })
=cut
sub find_file
{
    return __find_dirent('e', @_);
}

=item STRING find_dir (dirent_name: STRING)
=item STRING find_dir (dirent_name=>STRING[, no_default_path=>BOOL] [, dirs=>ARRAYREF])
=item STRING ffind_dir ({dirent_name=>STRING[, no_default_path=>BOOL] [, dirs=>ARRAYREF] })
=cut
sub find_dir
{
    return __find_dirent('d', @_);
}

=item STRING find_program (dirent_name: STRING)
=item STRING find_program (dirent_name=>STRING[, no_default_path=>BOOL] [, dirs=>ARRAYREF])
=item STRING ffind_program ({dirent_name=>STRING[, no_default_path=>BOOL] [, dirs=>ARRAYREF] })
=cut
sub find_program
{
    return __find_dirent('x', @_);
}

1;

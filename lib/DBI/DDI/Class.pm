package DBI::DDI::Class;

package DBI::DDI::Class::Accessor;
use base 'Class::Accessor';

sub set {
    my $self = shift;
    $self->SUPER::set(@_);
    return $self;
}

package DBI::DDI::Class;
use base 'DBI::DDI::Class::Accessor';
use strict;
use warnings;


my @rw_accessors = qw(
    dbh
    ddi
    ns
    impl
    charset
);

__PACKAGE__->mk_accessors(@rw_accessors);


=head2 new

    my $ddi = DBI::DDI->new(
        [-DDI=>$ddi]
        [,-DBH=>$dbh]
        [,-Impl=>$ddiImplPackage]
        [,-NS=>$ns]
        [,-Charset=>$charset]
    );

=cut


sub new {
    my ($class, %args) = @_;
    my $self = bless {}, $class;
    $self->dbh($args{'-DBH'})   if $args{'-DBH'};
    $self->ddi($args{'-DDI'})   if $args{'-DDI'};
    $self->ns($args{'-DDI'})    if $args{'-NS'};
    $self->impl($args{'-Impl'}) if $args{'-Impl'};
    $self->charset( $args{'-Charset'} || 'utf8');
    return $self;
}


sub prepare
{
    my $self = shift or die;
    my %args     = @_;
    my $ddi      = $args{-DDI} or die;
    my $prefix   = $args{-Prefix} || '';
    my $suffix   = $args{-Suffix} || '';

    # rename all table names with $prefix and $suffix
    foreach my $key ( keys %{$ddi} ) {
        $ddi->{$prefix.$key.$suffix} = delete $ddi->{$key};
    }
}



1;

__END__

=head2 dbh

Set/get database handler.

=head2 ddi

Set/get database DDI declaration.
    
=head2 ns

Set/get namespace (database name).

=head2 impl

Set/get package of DBI::DDI interface implementation.

=head2 charset

Set character set for specified namespace (database).

=cut

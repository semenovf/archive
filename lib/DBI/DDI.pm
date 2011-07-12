package DBI::DDI;
use Carp;
use Module::Load;
use DBI::DDI::Const;
use strict;
use warnings;

use base 'Exporter';

my @_ddi_tags = qw(
    DDI_PK
    DDI_INT
    DDI_BOOL
    DDI_TIME
    DDI_DATE
    DDI_DATETIME
    DDI_TIMESTAMP
    DDI_TEXT
    DDI_REF
    DDI_AUTOINC
    DDI_NOTNULL
    DDI_UNIQUE
);

our @EXPORT = qw(prepare deploy recall);
our @EXPORT_OK = @_ddi_tags;
our %EXPORT_TAGS = (
    ddi=>[@_ddi_tags]
);


=head2 prepare(-DDI=>$ddi [,-Prefix=>$prefix] [,-Suffix=>$suffix])

Add specified prefix and/or suffix to ...

=cut

sub prepare
{
    my %args     = @_;
    my $ddi      = $args{-DDI} or die;
    my $prefix   = $args{-Prefix} || '';
    my $suffix   = $args{-Suffix} || '';

    # rename all table names with $prefix and $suffix
    foreach my $key ( keys %{$ddi} ) {
        $ddi->{$prefix.$key.$suffix} = delete $ddi->{$key};
    }
}

=head2 deploy (-DDI=>$ddi, -DBH=>$dbh, -Impl=>$ddiImplPackage, -NS=>$ns [,-Charset=>$charset])

Deploy database

=cut

sub deploy
{
    my %args     = @_;
    my $ddi      = $args{-DDI} or croak ;
    my $ns       = $args{-NS} or die;
    my $dbh      = $args{-DBH} or die;
    my $ddiImpl  = $args{-Impl} or die;
    my $charset  = $args{-Charset} || 'utf8';

    load $ddiImpl;
    my @sql = $ddiImpl->spec_for_deploy(-DDI=>$ddi, -NS=>$ns, -Charset=>$charset);

    # Enable transactions (by turning AutoCommit off) until the next call to commit or rollback.
    # After the next commit or rollback, AutoCommit will automatically be turned on again.
    $dbh->begin_work or croak sprintf('transaction start: %s',$dbh->errstr);
    foreach ( @sql ) {
        print $_, "\n";
        $dbh->do($_);
        if ( $dbh->err ) {
            my $err = $dbh->err;
            my $errstr = $dbh->errstr;
            $dbh->rollback; # clear errors
            croak sprintf( 'DBI error [%d]: %s at SQL [%s]', $err, $errstr, $_);
        }
    }
    $dbh->commit;
    1;
}

=head2 recall(-DDI=>$ddi, -DBH=>$dbh, -Impl=>$ddiImplPackage, -NS=>$ns [,-DropDB=>0|1)

Recall (destroys) namespace (database) content and database itself
if -DropDB specified and is not equal to 0 and database has no tables in turn.

=cut

sub recall
{
    my %args = @_;
    my $ddi      = $args{-DDI} or die;
    my $ns       = $args{-NS} or die;
    my $dbh      = $args{-DBH} or die;
    my $ddiImpl = $args{-Class} or die;
    my $drop_db  = $args{-DropDb} || 0;

    load $ddiImpl;
    my @sql = $ddiImpl->spec_for_recall(-DDI=>$ddi, -NS=>$ns);

    # Enable transactions (by turning AutoCommit off) until the next call to commit or rollback.
    # After the next commit or rollback, AutoCommit will automatically be turned on again.
    $dbh->begin_work or croak sprintf('transaction start: %s',$dbh->errstr);
    foreach ( @sql ) {
        $dbh->do($_);
        if ( $dbh->err ) {
            my $err = $dbh->err;
            my $errstr = $dbh->errstr;
            $dbh->rollback;
            croak sprintf( 'DBI error [%d]: %s at SQL [%s]', $err, $errstr, $_);
        }
    }
    $dbh->commit;

    my $tab_names = get_tables($dbh, $ns);
    if( scalar(@{$tab_names}) == 0 && $drop_db ) {
        $dbh->do(sprintf('DROP DATABASE IF EXISTS %s', $ns));
        croak sprintf('%s while dropping database \'%s\'', $dbh->errstr, $ns) if $dbh->err;
    }
}


sub DDI_PK
{
    $_[0]->{-pk} = 1;
    return $_[0];
}

sub DDI_AUTOINC
{
    $_[0]->{-autoinc} = 1;
    return $_[0];
}

sub DDI_NOTNULL
{
    $_[0]->{-notnull} = 1;
    return $_[0];
}


sub DDI_INT
{
    return {
        -type=>DDI_TYPE_INT,
        -min=>defined $_[0] ? $_[0] : 0,
        -max=>defined $_[1] ? $_[1] : DDI_MAX_INT
    };
}

sub DDI_BOOL
{
    return {
        -type=>DDI_TYPE_BOOL,
    };
}

sub DDI_TIME
{
    return {
        -type=>DDI_TYPE_TIME
    };
}

sub DDI_DATE
{
    return {
        -type=>DDI_TYPE_DATE
    };
}


sub DDI_DATETIME
{
    return {
        -type=>DDI_TYPE_DATETIME
    };
}

sub DDI_TIMESTAMP
{
    return {
        -type=>DDI_TYPE_TIMESTAMP
    };
}

sub DDI_TEXT
{
    return {
        -type   => DDI_TYPE_TEXT,
        -length => defined $_[0] ? $_[0] : DDI_TEXT_MAXLEN
    };
}

sub DDI_REF($)
{
    return {
        -type=>DDI_TYPE_REF,
        -ref=>$_[0]
    };
}

sub DDI_UNIQUE
{
    my @args = @_;
    return {
        -type=>DDI_TYPE_INDEX_UNIQUE,
        -columns=>\@args
    };
}

sub get_tables
{
    my $dbh = shift or die;
    my $db_name = shift;
    if( $db_name ) {
        $dbh->do("USE $db_name") or croak $dbh->errstr;
    }
    my @tab_names = $dbh->tables('','','','TABLE');
    for( my $i = 0; $i < @tab_names; $i++ ) {
        $tab_names[$i] =~ /`([^`]+)`\.`([^`]+)`/ and do {
            $tab_names[$i] = $2;
            next;
        };
        croak 'unsupported pattern: ', $tab_names[$i];
    }
    return wantarray ? @tab_names : \@tab_names;
}

1;

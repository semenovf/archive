package JQ::DBI;
use DBI;
use strict;
use vars qw(@ISA);
@ISA = qw(DBI);

# the MyDBI::dr::connect method is NOT called!
# you can either override MyDBI::connect()
# or use MyDBI::db::connected()

package JQ::DBI::db;
use JQ::RC;
use strict;
use vars qw(@ISA);
@ISA = qw(DBI::db);

=item

  $rows = $dbh->do_cached($sql)           or die $dbh->errstr;
  $rows = $dbh->do_cached($sql, \%attr)   or die $dbh->errstr;
  $rows = $dbh->do_cached($sql, \%attr, @bind_values) or die $dbh->errstr

=cut

sub do_cached
{
    my($dbh, $sql, @bind_values) = @_;
    my $sth = $dbh->prepare_cached($sql) or
        return rc_fail sprintf('unable to prepare SQL statement: %s [%s]', $dbh->errstr, $sql);

    $sth->execute(@bind_values) or
        return rc_fail sprintf('SQL statement execution error [%d]: %s [%s]', $dbh->err, $dbh->errstr, $sql);

    return rc_ok( $sth->rows );
}


=item

  $rows = $dbh->do_cached_bulk( [$sql, @bind_values], ... );

=cut

sub do_cached_bulk
{
    my $dbh = shift;
    my $stmt_count = 0;
    $dbh->begin_work or return rc_failed( $dbh->errstr );
    foreach( @_ ) {
        my ( $sql, @bind_values ) = @{$_};
        my $rc = $dbh->do_cached($sql, @bind_values );
        if( !$rc->isOk ) {
            $dbh->rollback;
            return $rc;
        }
        $stmt_count++;
    }
    $dbh->commit;
    return rc_ok($stmt_count);
}


=item

    Return arrayref of hashrefs
    $rc = $sth->fetchall_hashrefs( [$sql, @bind_values], ... );

=cut

sub fetchall_hashrefs
{
    my($dbh, $sql, @bind_values) = @_;
    my $sth = $dbh->prepare_cached($sql) or
        return rc_fail sprintf('unable to prepare SQL statement: %s [%s]', $dbh->errstr, $sql);

    $sth->execute(@bind_values) or
        return rc_fail sprintf('SQL statement execution error [%d]: %s [%s]', $dbh->err, $dbh->errstr, $sql);

    my $all = [];
    while( my $hashref = $sth->fetchrow_hashref ) {
        push @{$all}, $hashref;
    }
    return rc_ok $all;
}

package JQ::DBI::st;
use vars qw(@ISA);
@ISA = qw(DBI::st);



1;

package Hg::Transaction;
use strict;
use warnings;
use Hg::Config;
use DBI qq(:sql_types);
use Data::Dumper;

sub new {
    my $class = ref $_[0] ? ref shift : shift;
    my %args = @_;
    my $self = {
          -id       => $args{-Id} || 0
        , -message  => $args{-Message} || ''
        , -priority => $args{-Priority} || 7
        , -fromHost => $args{-FromHost} || 'localhost'
    };
    
    return bless $self, $class;
}


sub toList
{
    my $self = shift;
    my $limit = shift || 10;
    my $dbh = __PACKAGE__->config->{-DBH} or die;
 
    my $sth = $dbh->prepare("SELECT ID, Message, Priority, FromHost FROM SystemEvents WHERE Priority < 4 ORDER BY ID DESC LIMIT ?");
    $sth->bind_param(1, $limit, { TYPE => SQL_INTEGER });
    $sth->execute;
    
    my @syslogList = ();
    
    while(1) {
        my $row = $sth->fetchrow_hashref;
        unless (defined $row) {
            die if $sth->err;
            last;
        }
        print Dumper $row, "\n";
        push @syslogList, Stk::SyslogEvent->new(
              -Id      => $row->{ID}
            , -Message => $row->{Message}
            , -Priority=> $row->{Priority}
            , -FromHost=> $row->{FromHost})
    };
    
    return \@syslogList;
}

sub input
{
    
}

1;
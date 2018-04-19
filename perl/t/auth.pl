#!/usr/bin/perl -w
#use JQ::DBI qw(:ddi);
use JQ::Auth::DBI;
use strict;

my $data_source = 'dbi:mysql:host=localhost:port=3306';
my $db_name     = 'jq_auth';
my $db_user     = 'root';
my $db_passwd   = '12345678';
my $db_scheme   = 'jq_auth';
my $db_attrs    = {
    'PrintError' => 0,
    'RaiseError' => 0,
    'mysql_enable_utf8' => 1
};

my $dbh = JQ::DBI->connect( $data_source, $db_user, $db_passwd, $db_attrs ) or
    die JQ::DBI->errstr;

if( $#ARGV > -1 ) {
    if( $ARGV[0] eq 'deploy' ) {
        do_deploy($db_scheme, $dbh);
    } elsif( $ARGV[0] eq 'destroy' ) {
        do_destroy($db_scheme, $dbh);
    } elsif( $ARGV[0] eq 'test' ) {
        do_tests();
    } else {
        print "Usage: perl auth.pl {deploy|destroy|test}";
    }
} else {
    print "Usage: perl auth.pl {deploy|destroy|test}";
}

$dbh->disconnect();


sub do_deploy
{
    my ($ns, $dbh ) = @_;
    return JQ::DBI->deploy(JQ::Auth::DBI->ddi, $ns, $dbh);
}

sub do_destroy
{
    my ($ns, $dbh ) = @_;
    return JQ::DBI->destroy(JQ::Auth::DBI->ddi, $ns, $dbh);
}

sub do_tests
{

}

0;

__END__

=head1 Heading text head1

=cut

=head2 Heading text head2

=cut

=head3 Heading text head3

=cut

=head4 Heading text head4

=cut


=over

=item Item1

Item1 inner text

=cut

=item Item2

I<Italic>
B<Bold>
C<Code>
L<Hyperlink>
10 E<lt> 20 E<gt> 15
F<filename>
S<text contains non-breaking spaces>
X<an index entry>

=cut

##
=item Item3

Item3 inner text

=cut

=item Item4

Item4 inner text

=cut

=back

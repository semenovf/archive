#!/usr/bin/perl -w
use strict;

use DBI;

# Connect to database (a MySQL database in this example)
use DBI;
my $database = 'inetlab_seo';
#my $dsn = "DBI:mysql:database=$database;mysql_read_default_group=client";
my $dsn = "DBI:mysql:mysql_read_default_group=client";
my $user = 'root';
my $pass = '12345678'; # Get password from /etc/my.cnf config file
my $dbh = DBI->connect($dsn, $user, $pass,
                       {'RaiseError' => 1});
if (!$dbh) {
    die "Can't connect to $dsn: $!";
}

$dbh->do("USE $database") or die $dbh->errstr;

my @tab_names = $dbh->tables('','','','TABLE');
for( my $i = 0; $i < @tab_names; $i++ ) {
    $tab_names[$i] =~ /`([^`]+)`\.`([^`]+)`/ and do {
        $tab_names[$i] = $2;
    };
}

print join( "\n", @tab_names );

#foreach (@tab_names) {
#    /`([^`]+)`\.`([^`]+)`/ and do {
#        my $db_name = $1;
#        my $tab_name = $2;
#        print $db_name, ': ', $tab_name, "\n";
#        next;
#    };
#    print $_, "\n";
#}

# Disconnect database connection
$dbh->disconnect;

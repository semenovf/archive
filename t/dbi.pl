#!/usr/bin/perl -w
use JQ::DBI;
use strict;

my $dbname = 'jq_test';
my $dsn    = 'DBI:mysql:host=localhost';
my $user   = 'root';
my $pass   = '12345678';
my $dbh    = JQ::DBI->connect($dsn, $user, $pass,
    {'RaiseError' => 0, 'PrintError'=>0})
    or die "Can't connect to $dsn: $!";

my $rc = $dbh->do_cached_bulk(
    [q(USE jq_test), {}],
    [q(BEGIN), {}],
    [q!CREATE TABLE IF NOT EXISTS tab0 (id INT, txt TEXT)!, {}],
    [q!CREATE TABLE IF NOT EXISTS tab1 (id INT, txt TEXT)!, {}],
    [q(ROLLBACK), {}],
);

unless ( $rc->isOk ) {
    warn 'ERROR: ', $rc->errstr, "\n";
} else {
    print $rc->rc, "\n";
}


$dbh->disconnect;

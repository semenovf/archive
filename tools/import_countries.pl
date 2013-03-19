#!/usr/bin/env perl
use Data::Dumper;
use FindBin;
BEGIN { unshift @INC, "$FindBin::Bin/../lib"; unshift @INC, "$FindBin::Bin/../tools" }
use CSV::Persist;
use strict;
use warnings;

# CSV format:
#  id,code,name,latitude,longitude,currency,timezone
#  or
#  code,name,latitude,longitude,currency,timezone
#  or
#  code,name,currency,timezone
#  or
#  code,name,currency
#
#  e.g.
#
#  1,"AD","Andorra",42.5,1.5,"EUR","Europe/Andorra"
#  2,"AE","United Arab Emirates",24,54,"AED","Asia/Dubai"
#  3,"AF","Afghanistan",33,65,"AFN","Asia/Kabul"
#  4,"AG","Antigua and Barbuda",17.05,-61.8,"XCD","America/Antigua"
#  ...
#  or
#
#  "AD","Andorra",42.5,1.5,"EUR","Europe/Andorra"
#  "AE","United Arab Emirates",24,54,"AED","Asia/Dubai"
#  "AF","Afghanistan",33,65,"AFN","Asia/Kabul"
#  "AG","Antigua and Barbuda",17.05,-61.8,"XCD","America/Antigua"
#  ...
#  or
#
#  "AD","Andorra",EUR","Europe/Andorra"
#  "AE","United Arab Emirates","AED","Asia/Dubai"
#  "AF","Afghanistan","AFN","Asia/Kabul"
#  "AG","Antigua and Barbuda","XCD","America/Antigua"
#  ...
#  or
#
#  "AD","Andorra",EUR"
#  "AE","United Arab Emirates"
#  "AF","Afghanistan","AFN"
#  "AG","Antigua and Barbuda","XCD"
#  ...

my $csv_file = '../../hg/rc/db/countries.csv';
$csv_file = $ARGV[0] if @ARGV > 0;

my $persist = CSV::Persist->new(Config=>'config.json', Prefix=>'countries');
$persist->persist($csv_file);

#$sth = $dbh->prepare('SELECT code, name, currency FROM '
#        . $config->{table_name_countries}) or die $dbh->errstr;
#$sth->execute or die $dbh->errstr;
#while (my $row = $sth->fetchrow_hashref) {
#    printf("%2s\t%3s\t%s\n", $row->{code}, $row->{currency}, $row->{name});
#}

0;
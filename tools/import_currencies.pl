#!/usr/bin/env perl
use Text::CSV::Encoded;
use strict;
use warnings;
use DBI;
use JSON;
use Data::Dumper;
use FindBin;
BEGIN { unshift @INC, "$FindBin::Bin/../lib" }

# Load config
my $config;
{
    local $/;
    open( my $fh, '<', 'config.json' );
    $config = decode_json(<$fh>);
    close($fh);
}

$config->{db_host} = 'localhost' unless $config->{db_host};
$config->{db_port} = 3306 unless $config->{db_port};
$config->{table_name_countries} = 'countries' unless $config->{table_name_countries};

#print Dumper $config;

# CSV format:
#  id,code,name,symbol,country
#  or
#  code,name,symbol,country
#  or
#  code,name,country
#
#  e.g.
#
# 11,"BAM","Convertible Mark","KM","BA"
# 12,"BBD","Barbados Dollar","$","BB"
# 13,"BDT","Taka","","BD"
#  ...
#  or
#
# "BAM","Convertible Mark","KM","BA"
# "BBD","Barbados Dollar","$","BB"
# "BDT","Taka","","BD"
#  ...
#  or
#
# "BAM","Convertible Mark","BA"
# "BBD","Barbados Dollar","BB"
# "BDT","Taka","BD"
#  ...

my $csv_file = '../../hg/rc/db/currencies.csv';
$csv_file = $ARGV[0] if @ARGV > 0;

open my $in, "<", $csv_file or die "$csv_file: $!";
my $dbh = DBI->connect('dbi:mysql:database='
        . $config->{db_name}
        . ';host=' . $config->{db_host}
        . ';port=' . $config->{db_port}
        , $config->{db_user}
        , $config->{db_password}
        , { RaiseError => 0, AutoCommit => 0 }) or die 'unable to disconnect to database';

print 'Connected to database by ', $config->{db_user}, '@', $config->{db_host}, ':', $config->{db_port}, "\n";

my $csv = Text::CSV::Encoded->new ({
      sep_char => ','
    , quote_char => '"'
    , binary => 1, eol => $/ # for supporting embedded newlines
    , encoding_in  => "utf8", 
    , encoding_out => "utf8", 
});

my $line = 0;
$dbh->do('DROP TABLE IF EXISTS ' . $config->{table_name_countries}) or die $dbh->errstr;
$dbh->do('CREATE TABLE IF NOT EXISTS '
        . $config->{table_name_countries}
        . '(`code` CHAR(2)  NOT NULL,
            `name` VARCHAR(45) NOT NULL,
            `currency` CHAR(3) NOT NULL,
            PRIMARY KEY (`code`)
            ) ENGINE=InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci' ) or die $dbh->errstr;

my $sth = $dbh->prepare('INSERT INTO '
        .  $config->{table_name_countries}
        . ' (code, name, currency) VALUES (?, ?, ?)') or die $dbh->errstr;

while (my $row = $csv->getline ($in)) {
    ++$line;
    my @fields = @$row;
    next unless @fields;
    next if $fields[0] =~ /^#/; # ommit comment lines (first column begins with '#')
    my $data = {code => undef, name => undef, currency => undef};
    SWITCH: {
        (@fields == 7) and do { $data->{code} = $fields[1]; $data->{name} = $fields[2]; $data->{currency} = $fields[5]; last; };
        (@fields == 6) and do { $data->{code} = $fields[0]; $data->{name} = $fields[1]; $data->{currency} = $fields[4]; last; };
        (@fields == 4) and do { $data->{code} = $fields[0]; $data->{name} = $fields[1]; $data->{currency} = $fields[2]; last; };
        (@fields == 3) and do { $data->{code} = $fields[0]; $data->{name} = $fields[1]; $data->{currency} = $fields[2]; last; };
        die 'invalid row format at line ' , $line, "\n";
    };

    die 'invalid country code at line ', $line, "\n" unless length($data->{code}) == 2;
    die 'invalid currency code at line ', $line, "\n" unless length($data->{currency}) == 3 || length($data->{currency}) == 0; # Antarctica has no currency
    
    #print join(' ', $data->{code},  $data->{name}, $data->{currency}), "\n";
    $sth->execute($data->{code},  $data->{name}, $data->{currency}) or die $dbh->errstr;
    
    undef $data;
}
close($in);
$dbh->commit;

$sth = $dbh->prepare('SELECT code, name, currency FROM '
        . $config->{table_name_countries}) or die $dbh->errstr;
$sth->execute or die $dbh->errstr;
while (my $row = $sth->fetchrow_hashref) {
    printf("%2s\t%3s\t%s\n", $row->{code}, $row->{currency}, $row->{name});
}

$dbh->disconnect;

0;
#!/usr/bin/perl -w
use Carp;
use DBI;
use JQ::DDI ':ddi';
use JQ::DDI::Const;
use JQ::Logger;
use strict;
$SIG{ __DIE__ } = sub { Carp::confess( @_ ) };

binmode STDOUT, ":encoding(utf8)";

logging_debug(1);

my $ddi = {
    'vacancy' => {
        'id'              => DDI_PK(0, DDI_MAX_INT),
        'vid'             => DDI_TEXT(128),
        'link'            => DDI_TEXT(256),
        'text'            => DDI_TEXT,
        'text_normalized' => DDI_TEXT
    }
    #DDI_INDEX( {-Name=>'IndexName', -Kind=>DDI_INDEX_KIND_UNIQUE, -Type=>DDI_INDEX_TYPE_HASH}
};

my $db_ns     = 'quest';
my $db_source = 'dbi:mysql:host=localhost';
my $db_user   = 'root';
my $db_passwd = '12345678';
my $db_attrs  = {
    'PrintError' => 0,
    'RaiseError' => 0,
    'mysql_enable_utf8' => 1 #!Note: http://en.wikibooks.org/wiki/Perl_Programming/Unicode_UTF-8
};
my $dbh = DBI->connect( $db_source, $db_user, $db_passwd, $db_attrs ) or croak DBI->errstr;
JQ::DDI->prepare(-DDI=>$ddi, -Prefix=>'jq_', -Suffix=>'');
#JQ::DDI->deploy(-DDI=>$ddi, -NS=>$db_ns, -DBH=>$dbh, -Class=>'JQ::DDI::MySQL');
JQ::DDI->destroy(-DDI=>$ddi, -NS=>$db_ns, -DBH=>$dbh, -Class=>'JQ::DDI::MySQL', -DropDb=>1);
$dbh->disconnect;

0;

__END__

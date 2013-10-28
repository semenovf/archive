#!/usr/bin/perl -w

use File::Basename;
use Getopt::Long;
use Pod::Usage;
use URI;
use Q::Logger;
use HgQuest::Parser;
use XML::Parser;
use JobQuest::Conf;

use strict;

our $PROGRAM = 'HgQuest';
our $VERSION = '2.0a';
our $COPYLEFT = "$PROGRAM. Version $VERSION. Copyright (C) 2007,2008,2013. Semenov Fedor (TM)";

# Signal handlers {{{
#
$SIG{__WARN__} = sub { cwt::Logger::warn('PERL SYSTEM WARNING: ', @_); };
#$SIG{__DIE__}  = sub {
#    if ( $^S ) { # Executing an eval. So do not output error message because it rised at CATCH block 
#        return;
#    }
#    jq_fatal( @_ );
#    exit 1;
#};

#
# }}} Signal handlers



# {{{ 
my $CONF = 'jqconf.xml';
my $HELP = 0;
my $MAN = 0;
my $DEBUG = 0;
my $RUN_AS_DAEMON = 0; ## Command line options
our $ENC = 'cp1251';

my $__sig = $SIG{__WARN__};
$SIG{__WARN__} = sub { print STDERR "PARSE ERROR: @_\n" };
my $rc = GetOptions(
        "conf=s"  => \$CONF,
        "enc=s"   => \$ENC,
        "verbose" => \$DEBUG,
        "help|?"  => \$HELP,
        "man"     => \$MAN,
        "daemon"  => \$RUN_AS_DAEMON ) or pod2usage(0);
$SIG{__WARN__} = $__sig;

if( $HELP ) {
    print "\n", $COPYLEFT, "\n\n";
    pod2usage( -exitval=>1, -verbose=>1, -output=>\*STDERR );
    exit 1;
}

if( $MAN ) {
    print "\n", $COPYLEFT, "\n\n";
    pod2usage( -exitval=>1, -verbose=>2, -output=>\*STDERR );
    exit 1;
}
# }}} 


# turn on/off the output of the debugging information
jq_is_debug($DEBUG);
my %AppData = jq_load_conf($CONF);

foreach my $parser ( @{$AppData{'parsers'}} ) {
    $parser->parse;
}

0;

#my $writer = JobQuest::Writer->new('CSVWriter', -path=>'d:\\temp\\vacancy.csv');
#my $sqlwriter = JobQuest::Writer->new('SQLWriter',
#        -dbname=>'jobquest',
#        -host=>'localhost', -port=>5432, -user=>'jq_editor', -pass=>'1qazxsw2');
#
#my $parser1 = JobQuest::Parser->new('www_e1_ru',
#    -pages=>6,
#    -reader=>JobQuest::Reader->new('HTTPUrgentReader', -Scheme=>[5, 10], -path=>URI->new(
#        'http://www.e1.ru/business/job/vacancy.search.php?'.
#		'section=10&sex=l&search_by=1&show_for=7&order_by=2&search=yes&salary_currency=1&page=0') ),
#    -writer=>$writer );
#
#my $parser2 = JobQuest::Parser->new('www_rabota66_ru',
#    -pages=>4,
#    -reader=>JobQuest::Reader->new('HTTPUrgentReader', -Scheme=>[5, 10],
#        -path=>URI->new('http://www.rabota66.ru/vacancy/vbranch6') ),
#    -writer=>$writer );
#
#my $parser3 = JobQuest::Parser->new('www_ekat_ru',
#    -pages=>4,
#    -reader=>JobQuest::Reader->new('HTTPUrgentReader', -Scheme=>[5, 10],
#        -path=>URI->new('http://www.ekat.ru/job/?cmd=vaclst&rid=3&p=1') ),  # IT
#    -writer=>$writer );


#
#
# itemsOnPage=50 - количество вакансий на страницу
# isFromAgency=10 - выводить вакансии от агентств
# isWithoutSalary=10 - выводить вакансии без указания зарплаты
# orderBy=2 - сортировать по дате (2)
# actionSearch=search&
# areaId=3  - Екатеринбург и область
# keyword1=
# searchPeriod=7    - вывод за последнюю неделю (7)
# professionalAreaId=1 Професиональная область (1-Информационные технологии/Интернет/Телеком)
#my $parser4 = JobQuest::Parser->new('ural_hh_ru',
#    -pages=>4,
#    -reader=>JobQuest::Reader->new('HTTPUrgentReader', -Scheme=>[5, 10],
#        -path=>URI->new('http://ural.hh.ru/applicant/vacancySearch.do?itemsOnPage=10&isFromAgency=10&isWithoutSalary=10&orderBy=2&actionSearch=search&areaId=3&keyword1=&searchPeriod=7&professionalAreaId=1') ),  # IT
#    -writer=>$writer );

#my $parser3 = JobQuest::Parser->new('www_ekat_ru',
#    -pages=>4,
#    -reader=>JobQuest::Reader->new('HTTPUrgentReader', -Scheme=>[5, 10],
#        -path=>URI->new('http://www.ekat.ru/job/?cmd=vaclst&rid=3&hot=0&p=2') ),  # IT
#    -writer=>$writer );

#my $parser03 = JobQuest::Parser->new('www_ekat_ru',
#    -pages=>2,
#    -reader=>JobQuest::Reader->new('FileReader',
#        -path=>'file:///D:/temp/ekat_ru/www.ekat.ru/job/index136a.html' ),  # IT
#    -writer=>$writer );
#
#my $parser04 = JobQuest::Parser->new('www_ekat_ru',
#    -pages=>2,
#    -reader=>JobQuest::Reader->new('FileReader',
#        -path=>'file:///D:/temp/ekat_ru/www.ekat.ru/job/index136a.html' ),  # IT
#    -writer=>$sqlwriter );


__END__

=head1 NAME

jq - main program of JobQuest.ru project

=head1 SYNOPSIS

perl jq --conf=PATH [options]

=head1 OPTIONS

    --conf=PATH     configuration file path
    --enc=ENC       internal encoding
    --verbose       verbosely output (output debug messages)
    --daemon        run as daemon
    --help | ?      this help message
    --man           manual

=head1 DESCRIPTION

B<jq> main program of JobQuest.ru project.

=cut





__END__

# This is a first thread
my $jobs = new JobQuest::JobManager(
	[$job_e1_ru, $job_rabota66_ru] );


if( $RUN_AS_DAEMON ) {
    my $pid = fork();
    die "unable to run as daemon: $!\n" unless defined($pid);
    
    if( $pid ) { ## parent process
        jq_info( "child process started with pid: $pid" );
        exit 0;
    }
}
$jobs->run;
undef $jobs;

0;

__END__

# signal handlers
$SIG{'TERM'}   = sub { jq_info("$PROG_NAME: TERM signal has been rised, wait some time to safely terminate the program"); };
$SIG{'HUP'}    = sub { jq_info("$PROG_NAME: HUP signal has been rised, nothing to do yet"); };
# Ctrl+C
$SIG{'INT'}    = sub { jq_info("$PROG_NAME: INT signal has been rised, wait some time to safely terminate the program"); };
# Ctrl+Break
$SIG{'BREAK'}  = sub { jq_info("$PROG_NAME: BREAK signal has been rised, wait some time to safely terminate the program"); };

my $USAGE = sub {
    print <<USAGE;
    
$COPYLEFT 
Usage: $PROGRAM [OPTIONS]
OPTIONS:                
    --help|?             this help usage
    --version            program version
    --conf|-c CONF       configuration file
    --cache DIR          cache directory
    --days NUM           number of days vacancy published
    --jqid JQID          vacancy id which stops processing
    
USAGE
#    exit 0;
}; ## end of USAGE

# {{{ ################# PARSE COMMAND LINE ( to find 'help' or 'conf' options )
#


jq_logger_init("e1_ru",
	\qq{
        log4perl.logger                 = DEBUG, File, Screen

        log4perl.appender.File          = Log::Log4perl::Appender::File
	log4perl.appender.File.Threshold = ERROR
        log4perl.appender.File.layout   = Log::Log4perl::Layout::PatternLayout
        log4perl.appender.File.layout.ConversionPattern = [%d{yyyy.MM.dd HH:mm:ss}] <%c{1}>: %p: %m%n
        log4perl.appender.File.filename = d:\\temp\\jq.log

        log4perl.appender.Screen         = Log::Log4perl::Appender::Screen
        log4perl.appender.Screen.layout  = Log::Log4perl::Layout::PatternLayout
        log4perl.appender.Screen.layout.ConversionPattern = <%c{1}>: %5p: %m%n
        log4perl.appender.Screen.stderr  = 1

        log4perl.appender.Mail           = Log::Dispatch::Email::MailSender
        log4perl.appender.Mail.Threshold = ERROR
        log4perl.appender.Mail.layout    = Log::Log4perl::Layout::PatternLayout
        log4perl.appender.Mail.layout.ConversionPattern = <%c{1}>: %p: %m%n
        log4perl.appender.Mail.subject   = Aspd::Mail::Spooler
        log4perl.appender.Mail.to        = wladt\@e-burg.cbr.ru
        log4perl.appender.Mail.from      = spooler_aspd\@e-burg.cbr.ru
        log4perl.appender.Mail.smtp      = pg
    });
#
# }}} ############### PARSE COMMAND LINE


0;

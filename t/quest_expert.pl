#!/usr/bin/perl -w
use strict;
use JQ::Web::JobQuest;
use JQ::Logger;
use JQ::UserAgent;
use Carp;
$SIG{ __DIE__ } = sub { Carp::confess( @_ ) };

binmode STDOUT, ":encoding(utf8)";

logging_debug(1);

my $ua = JQ::UserAgent->new;
$ua->agent('Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.9.1.9) Gecko/20100315 Firefox/3.5.9');
$ua->timeout(30);
$ua->parse_head(0); # for disable warning: Parsing of undecoded UTF-8 will give...
$ua->httpProxy('ENV');
$ua->cacheDir ('./cache');
$ua->cacheOnDemand(1);

#my $plan = JQ::UserAgent::Plan::TimedOut->new;
#$plan->timeout(5);
#$ua->addPlan($plan);

#$ua->cacheEncoding( );
#$ua->cookie({file=>$self->{'Quest.UserAgent.cookie'}, autosave=>0});


my $jobQuest = JQ::Web::JobQuest->new;
$jobQuest->userAgent($ua);

my $expert;
$expert = $jobQuest->newExpert('rabota66.ru:IT', 'JQ::Web::Quest::Expert::Job::rabota66_ru'); #JQ::Web::Quest::Expert::Job::rabota66_ru->new();
$expert->url('http://www.rabota66.ru/vacancy/vbranch6');
$expert->pagesLimit(1);
#$expert->daysLimit(2);

$expert = $jobQuest->newExpert('e1.ru:IT', 'JQ::Web::Quest::Expert::Job::e1_ru'); #JQ::Web::Quest::Expert::Job::e1_ru->new();
$expert->url('http://www.e1.ru/business/job/vacancy.search.php?search=yes&section=10');
$expert->pagesLimit(1);
#$expert->daysLimit(2);

$jobQuest->setVacancyHook( sub {
    my $v = shift or die;
    printf "Title: %s\n", $v->{-Title};
    printf "VID (vacancy Id): %s\n", $v->{-VID};
    printf "Link: %s\n", $v->{-Link};
    print $v->{-Text}, "\n";
});

unless( $jobQuest->isError ) {
    $jobQuest->exec();
}

if( $jobQuest->isError ) {
    foreach my $e (@{$jobQuest->errors}) {
        log_error($e);
    }
}
log_info('Finished');

0;

__END__

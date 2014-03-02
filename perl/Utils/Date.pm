package JQ::Utils::Date;
use Carp;
use Time::Local;
use strict;
use utf8;

require Exporter;
our @ISA = qw(Exporter);
our @EXPORT_OK = qw(
	ymd_date
	ymd_today
	ymd_yesterday
	ymd_month_str2int
	ymd_daysdiff
);

sub ymd_date
{
    my @time = localtime(time);
    #       year           month       day
#    return ($time[5]+1900, $time[4]+1, $time[3]) if wantarray;

	my $day   = shift || $time[3];
	my $month = shift || $time[4]+1;
	my $year  = shift || $time[5]+1900;

	return 10000*$year + 100*$month + 1*$day;
}

#sub jq_date_str
#{
#	my $date = shift;
#	return "$date";
#}

sub ymd_today
{
    my @time = localtime(time);
    #       year           month       day
    return ($time[5]+1900, $time[4]+1, $time[3]) if wantarray;
    return 10000*($time[5]+1900) + 100*($time[4]+1) + 1*$time[3];
}

sub ymd_yesterday
{
    my @time = localtime(time-86400);
	#       year           month       day
	return ($time[5]+1900, $time[4]+1, $time[3]) if wantarray;
    return 10000*($time[5]+1900) + 100*($time[4]+1) + 1*$time[3];
}

sub ymd_month_str2int
{
	$_ = lc $_[0];
	/СЏРЅРІР°СЂ/			and return 1;
	/С„РµРІСЂР°Р»/ 		and return 2;
	/РјР°СЂС‚/     		and return 3;
	/Р°РїСЂРµР»/			and return 4;
	/РјР°[РµР№СЋСЏ]Рј?'/	and return 5;
	/РёСЋРЅ/			and return 6;
	/РёСЋР»/      		and return 7;
	/Р°РІРіСѓСЃС‚/		and return 8;
	/СЃРµРЅС‚СЏР±СЂ/		and return 9;
	/РѕРєС‚СЏР±СЂ/		and return 10;
	/РЅРѕСЏР±СЂ/			and return 11;
	/РґРµРєР°Р±СЂ/		and return 12;
	croak 'invalid month representation string: '.$_;
}


=item ymd_daysdiff(date1, date2)

date1, date2 - calculate difference in days between two dates
dates are in format 'YYYYMMDD'

=cut

sub ymd_daysdiff
{
	my $date1 = shift;
	my $date2 = shift;

    my $y1 = int($date1/10000);
    my $m1 = int(($date1-$y1*10000)/100);
    my $d1 = $date1-$y1*10000-$m1*100;

    my $y2 = int($date2/10000);
    my $m2 = int(($date2-$y2*10000)/100);
    my $d2 = $date2-$y2*10000-$m2*100;

	my $t1 = timelocal(0,0,0,$d1,$m1-1,$y1-1900);
	my $t2 = timelocal(0,0,0,$d2,$m2-1,$y2-1900);

#    jq_debug($date2, '-', $date1, '=', int(($t2-$t1)/86400), ' date[s]');
	return int(($t2-$t1)/86400);
}

1;

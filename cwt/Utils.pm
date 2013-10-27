package JQ::Lib::Utils;

use Time::Local;
use cwt::Logger;
use cwt::Trim;
use strict;

#require Exporter;
#our @ISA = qw(Exporter);
#our @EXPORT_OK = qw(
#	jq_email_group_matched
#	jq_date jq_date_str jq_month_num jq_today jq_yesterday jq_daysdiff
#	xpath_text xpath_attr xpath_node
#    jq_max_array_strlen
#);
#our %EXPORT_TAGS = (
#	xpath => [qw(xpath_text xpath_attr xpath_node)]
#);

=item cwt::Utils::email_group_matched( $emailstring )

Trims leftmost, rightmost and both spaces
Packs spaces

=cut

sub email_group_matched
{
	my $email_addr_group = shift or return;
	
	my $RE_EMAIL_WORD = '[\w\d_][-\w\d_]*';    
	my $RE_EMAIL 	  = qq/${RE_EMAIL_WORD}(\.${RE_EMAIL_WORD})*\@${RE_EMAIL_WORD}(\.${RE_EMAIL_WORD})*/;
	my $RE_EMAIL_GROUP = qq/${RE_EMAIL}(\;${RE_EMAIL})*/;
	
	# $email_addr_group =~ s/ //g;
	return 1 if $email_addr_group =~ /$RE_EMAIL_GROUP/;
	return 0;
}

sub date
{
	my $year = shift;
	my $month = shift;
	my $day = shift;
	
	return 10000*$year + 100*$month + 1*$day;
}

sub date_str
{
	my $date = shift;
	return "$date";
}

sub today
{
    my @time = localtime(time);
    #       year           month       day
    return ($time[5]+1900, $time[4]+1, $time[3]) if wantarray;
    return 10000*($time[5]+1900) + 100*($time[4]+1) + 1*$time[3];
}

sub yesterday
{
    my @time = localtime(time-86400);
	#       year           month       day
	return ($time[5]+1900, $time[4]+1, $time[3]) if wantarray;
    return 10000*($time[5]+1900) + 100*($time[4]+1) + 1*$time[3];
}

sub month_num
{
	$_ = lc $_[0];
	/€нвар/			and return 1;
	/феврал/ 		and return 2;
	/март/     		and return 3;
	/апрел/			and return 4;
	/ма[ейю€]м?'/	and return 5;
	/июн/			and return 6;
	/июл/      		and return 7;
	/август/		and return 8;
	/сент€бр/		and return 9;
	/окт€бр/		and return 10;
	/но€бр/			and return 11;
	/декабр/		and return 12;
	return -1;
}


=item daysdiff(date1, date2)

date1, date2 - calculate difference in days between two dates
dates are in format 'YYYYMMDD'

=cut

sub daysdiff
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
    
      # завершить цикл просмотра страниц, если дата публикации вакансии превышает
      # заданное количество дней с текущей даты
    #  my ($d, $m, $y) = (localtime())[3,4,5];
    #  my $dateToday = sprintf("%02d.%02d.%d", $d, $m+1, $y+1900);
    #  if( jq_daysdiff($self->{vacancy}->[jqVPDATE], $dateToday) > $self->{days} ) {
    #      $self->{stopOk} = 1;
    #      return;
    #  }


=item xpath_text($node, $path, $index)
=item xpath_text($node, $path)
=item xpath_text($node)
	¬озвращает значение текстового элемента
=cut
sub xpath_text
{
	my $node = shift;
	my $path = shift || '.';
	my $index = shift || 0;
	my @l;

	@l = xpath_node( $node, $path.'/text()' );
	
	return unless @l;
	
	if( wantarray ) {
		my @strings = ();
		foreach( @l ) {
			$_ = $_->getValue;
			jq_sppacktrim(\$_);
			push(@strings, $_);
		}
		return @strings;
	}

	if( $index < 0 || $index >= scalar @l) {
		die("index [$index] out of bounds\n");
	}
	
	$_ = $l[$index]->getValue;
	jq_sppacktrim(\$_);

	return $_;
}

=item xpath_attr($node, $path, $attr_name, $index)
=item xpath_attr($node, $path, $attr_name)
	¬озвращает значение указанного атрибута или undef
=cut
sub xpath_attr
{
	my $node = shift;
	my $path = shift;
	my $attr_name = shift;
	my $index = shift || 0;
	
	my $attr = $node->findnodes($path)->[$index] or do {
#		jq_error("xpath [$path] with index [$index] not found");
		return undef;
	};
	return $attr->{$attr_name};
}

sub xpath_node
{
	my $node  = shift;
	my $path  = shift;
	my $index = shift || 0;
	
	my @l = $node->findnodes($path) or do {
#		jq_error("xpath [$path] with index [$index] not found");
		return () if wantarray;
		return undef;
	};
	
	return @l if wantarray;

	if( $index < 0 || $index >= length(@l)) {
		jq_error("index [$index] out of bounds");
		return undef;
	}

	return $l[$index];
}


# ќпределение максимальной длины строки из массива
sub max_array_strlen
{
    my $a;  # ссылка на массив строк
    my $max = 0;
    my $l;
    
    while(1) {
        if( scalar(@_) > 0 ) {
            if( scalar(@_) > 1 ) {
                $a = \@_;
            } elsif( ref($_[0]) eq 'ARRAY' ) {
                $a = $_[0];
            }
            last;
        }
        
        # function usage error handler
        die 'usage: max_array_strlen {ARRAYREF | str0, str1, str2,...}';
    }
    
    foreach my $s ( @$a ) {
        $l = length($s);
        $max = $l > $max ? $l : $max;
    }
    
    return $max;
}


#sub jq_print_to_var { my ($m, $v) = @_; local *STDOUT; open(STDOUT, ">", \$v); print $m; return $v; }

1;

package JQ::Web::Quest::Expert::Job::rabota66_ru;
use URI;
use JQ::Logger;
use JQ::Html2Text;
use JQ::Trim;
#use JQ::Utils::Date qw(ymd_date ymd_today ymd_month_str2int);
use base 'JQ::Web::Quest::JobExpert';
use strict;
use warnings;
use utf8;

sub new
{
    my $class = shift or die;
    my $self = $class->SUPER::new();
    return bless $self, $class;
}

sub nextPageHRef($$)
{
    my ($self, $html_tree) = @_;

    my $node = $html_tree->findnodes('/html/body//table[@class="vac-middleBlock"]')->[0] or do {
        $self->inconsistent([__PACKAGE__, __LINE__]);
        return;
    };

    my $pg = $node->findnodes('//img[@src="/tpl/i/icon_arrow_r.gif"]')->[0] or do {
        unless ( $node->findnodes('//table[@class="pager"]') ) {
            $self->inconsistent([__PACKAGE__, __LINE__]);
        }
        return;
    };

    $pg = $pg->parent()->findnodes('a')->[0] or do {
        $self->inconsistent([__PACKAGE__, __LINE__]);
        return;
    };

    return $pg->{'href'};
}


sub parsePage($$)
{
    my ($self, $html_tree) = @_;
    my @vacs = $html_tree->findnodes('/html/body//h2[@class="title"]/a[@class="vacancy"]') or do {
        $self->inconsistent([__PACKAGE__, __LINE__]);
        return 0;
    };

    foreach my $v (@vacs) {
        my $u = URI->new_abs($v->{'href'}, $self->url);
        $self->quest->addVLink($self, $u->canonical); # if $self->quest->can('addVLink');
    }

    1;
}

sub parseVacancy($$)
{
    my ($self, $html_tree) = @_;

    my $vac = $html_tree->findnodes('/html/body/table//div[@class="vac-item"]')->[0] or do {
        $self->inconsistent([__PACKAGE__, __LINE__]);
        return 0;
    };

    #my $date = $vac->findnodes('//td[@class="vac-item-head-nw"]')->[0] or do {
    #    $self->inconsistent([__PACKAGE__, __LINE__]);
    #    return 0;
    #};
    #
    #$date = lc sppacktrim( html2text($date) );
    #
    #if( $date =~ /РґРѕР±Р°РІР»РµРЅРѕ:\s+СЃРµРіРѕРґРЅСЏ/ ) {
    #    $date = ymd_today();
    #    log_debug('Date: today');
    #} elsif( $date =~ /РґРѕР±Р°РІР»РµРЅРѕ:\s+(\d\d?)\s+(\w+)/ ) {
    #    $date = ymd_date($1, ymd_month_str2int($2));
    #    log_debug_f('Date: %s', $date);
    #} else {
    #    $self->inconsistent([__PACKAGE__, __LINE__]);
    #    return 0;
    #}

    my $title = $vac->findnodes('//h1[@class="title"]')->[0] or do {
        $self->inconsistent([__PACKAGE__, __LINE__]);
        return 0;
    };

    my @list = $title->content_list;
    unless( scalar(@list) == 2 ) {
        $self->inconsistent([__PACKAGE__, __LINE__]);
        return 0;
    }
    $title = sppacktrim($list[1]);
    my $laquo = chr(171);
    my $raquo = chr(187);
    $title =~ s/^$laquo(.*)$raquo$/$1/g;
    my $text = html2text($vac);

    #log_debug_f('Date: %d', $date);
    #return {-Title=>$title, -Text=>$text, -Date=>$date};
    return {-Title=>$title, -Text=>$text};
}

1;

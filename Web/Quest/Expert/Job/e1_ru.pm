package JQ::Web::Quest::Expert::Job::e1_ru;
use URI;
use JQ::Logger;
use JQ::Html2Text;
use JQ::Trim;
use JQ::Utils::Date qw(ymd_date ymd_today ymd_month_str2int);
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

    my $pg = $html_tree->findnodes('/html/body//a/img[@src="/images/arrows/_arrow_middle_orange.gif"]')->[0] or do {
        $self->inconsistent([__PACKAGE__, __LINE__]);
        return;
    };

    my $href = $pg->parent()->{'href'} or do {
        $self->inconsistent([__PACKAGE__, __LINE__]);
        return;
    };

    return $href;
}


sub parsePage($$)
{
    my ($self, $html_tree) = @_;
    my @vacs = $html_tree->findnodes('/html/body//a[@class="big"]') or do {
        $self->inconsistent([__PACKAGE__, __LINE__]);
        return 0;
    };

    my $n = 0;
    foreach my $v (@vacs) {
        if( $v->{'href'} =~ /vacancy\.detail\.php\?id/ ) {
            $n++;
            my $u = URI->new_abs($v->{'href'}, $self->url);
            $self->quest->addVLink($self, $u->canonical); # if $self->quest->can('addVLink');
        }
    }

    unless( $n == 10 ) {
        $self->inconsistent([__PACKAGE__, __LINE__]);
        return 0;
    }

    1;
}

sub parseVacancy($$)
{
    my ($self, $html_tree) = @_;

    my $vac = $html_tree->findnodes('//strong[@class="big"]')->[0] or do {
        $self->inconsistent([__PACKAGE__, __LINE__]);
        return 0;
    };

    my $title = $vac->findnodes('//span[@class="big"]')->[0] or do {
        $self->inconsistent([__PACKAGE__, __LINE__]);
        return 0;
    };

    $vac = $vac->parent->parent->parent->parent->parent->parent;

    $title = sppacktrim(html2text($title));
    my $text = html2text($vac);

    #my $date = ymd_today();
    #if( $text =~ /Р”Р°С‚Р°:\s*(\d\d?)\s+(\w+)\s+(\d\d\d\d)/g ) {
    #    $date = ymd_date($1, ymd_month_str2int($2), $3);
    #    log_debug_f('Date: %s', $date);
    #} else {
    #    $self->inconsistent([__PACKAGE__, __LINE__]);
    #    return 0;
    #}

    #return {-Title=>$title, -Text=>$text, -Date=>$date};
    return {-Title=>$title, -Text=>$text};
}

1;

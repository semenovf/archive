package JQ::Web::Quest::Expert::Job::ural_hh_ru;
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

    my @pg = $html_tree->findnodes('/html/body//div[@class="b-pager"]/ul/li') or do {
        $self->inconsistent([__PACKAGE__, __LINE__]);
        return;
    };

    my $ok = 0;
    my $href;
    foreach ( @pg ) {
        my $c = $_->content_refs_list;
        if ( ref $c eq 'SCALAR' ) {
            $ok = 1;
        } else {
            if( $ok ) {
                $href = $$c->{'href'};
                last;
            }
        }
    }
    #my $href = $pg->parent()->{'href'} or do {
    #    $self->inconsistent([__PACKAGE__, __LINE__]);
    #    return;
    #};

    return $href;
}


sub parsePage($$)
{
    my ($self, $html_tree) = @_;
    my $node = $html_tree->findnodes('/html/body//div[@id="ie8slice-search"]')->[0] or do {
        $self->inconsistent([__PACKAGE__, __LINE__]);
        return 0;
    };

    my @vacs = $node->findnodes('.//a[@class="b-vacancy-list-link b-marker-link"') or do {
        $self->inconsistent([__PACKAGE__, __LINE__]);
        return 0;
    };

    my $n = 0;
    foreach my $v (@vacs) {
        log_debug($v->{'href'});
        my $u = URI->new_abs($v->{'href'}, $self->url);
        $self->quest->addVLink($self, $u->canonical);# if $self->quest->can('addVLink');
    }

    1;
}

sub parseVacancy($$)
{
    my ($self, $html_tree) = @_;

    my $title = $html_tree->findnodes('//h1[@class="b-title b-vacancy-title"]')->[0] ||
                $html_tree->findnodes('//h1[@class="hht-vacancy-title"]')->[0]
    or do {
        $self->inconsistent([__PACKAGE__, __LINE__]);
        return 0;
    };
    $title = $title->as_trimmed_text;

    my $text1 = $html_tree->findnodes('//div[@class="b-important b-vacancy-info"]')->[0] ||
                $html_tree->findnodes('//div[@class="hht-vacancy-bar g-round m-round_5"]')->[0]
    or do {
        $self->inconsistent([__PACKAGE__, __LINE__]);
        return 0;
    };
    $text1 = html2text($text1);

    my $text2 = $html_tree->findnodes('//div[@id="hypercontext"]')->[0] ||
                $html_tree->findnodes('//div[@class="hht-vacancyboxshadow"]')->[0]
    or do {
        $self->inconsistent([__PACKAGE__, __LINE__]);
        return 0;
    };
    $text2 = html2text($text2);

    return {-Title=>$title, -Text=>$text1."\n\n".$text2};
}

1;

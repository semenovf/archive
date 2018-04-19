package JQ::Web::Quest::Expert::SearchEngine::yandex_ru;
use URI;
use JQ::Logger;
use JQ::Html2Text;
use JQ::Trim;
use base 'JQ::Web::Quest::SearchEngineExpert';
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

    my $href = $html_tree->findnodes( '//a[@id="next_page"]' )->[0] or do {
        $self->inconsistent([__PACKAGE__, __LINE__]);
        return;
    };
    return $href->{'href'};
}


sub parsePage($$)
{
    my ($self, $html_tree) = @_;

    my @items = $html_tree->findnodes('//li[@class="b-serp-item"]') or do {
        $self->inconsistent([__PACKAGE__, __LINE__]);
        return 0;
    };

    foreach my $item (@items) {
        my $href = $item->findnodes('.//a[@class="b-serp-item__title-link"]')->[0]  or do {
            $self->inconsistent([__PACKAGE__, __LINE__]);
            return 0;
        };
        my $u = URI->new($href->{'href'});
        return 0 if $self->callback( -Host=>$u->host, -HRef=>$u->canonical );
    }

    1;
}

1;

package JQ::Web::Quest::Expert::SearchEngine::google_ru;
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

    my $href = $html_tree->findnodes( '//a[@id="pnnext"]' )->[0] or do {
        $self->inconsistent([__PACKAGE__, __LINE__]);
        return;
    };
    return $href->{'href'};
}


sub parsePage($$)
{
    my ($self, $html_tree) = @_;

    my @items = $html_tree->findnodes('//div[@id="ires"]/ol/li[@class="g"]') or do {
        $self->inconsistent([__PACKAGE__, __LINE__]);
        return 0;
    };

    foreach my $item (@items) {
        next if defined $item->id && $item->id eq 'newsbox';
        next if defined $item->id && $item->id eq 'lclbox';
        my $href = $item->findnodes('.//span[@class="tl"]/h3[@class="r"]/a')->[0]  or do {
            $self->inconsistent([__PACKAGE__, __LINE__]);
            return 0;
        };
        my $u = URI->new($href->{'href'});
        return 0 if $self->callback( -Host=>$u->host, -HRef=>$u->canonical );
    }

    1;
}

1;

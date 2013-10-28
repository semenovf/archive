package JQ::UserAgent::Plan::MultipleProxies;
use JQ::Logger;
use base qw(JQ::UserAgent::Plan);
use strict;
use warnings;

__PACKAGE__->mk_accessors(qw(
    _proxies
));


use constant PROXY_STATUS_UNKNOWN => 0;  # Г‘ГІГ ГІГіГ± Г­ГҐГЇГ°Г®ГўГҐГ°ГҐГ­Г­Г»Г©, ГЇГ°ГЁГ±ГўГ ГЁГўГ ГҐГІГ±Гї ГЇГ°ГЁ ГЁГ­ГЁГ¶ГЁГ Г«ГЁГ§Г Г¶ГЁГЁ Г¤Г®ГЎГ ГўГ«ГҐГ­ГЁГЁ ГЇГ°Г®ГЄГ±ГЁГЁ
use constant PROXY_STATUS_OK      => 1;  # ГЏГ°Г®ГЄГ±ГЁ ГЇГ°Г®ГўГҐГ°ГҐГ­ ГЁ Г®Г­ Г°Г ГЎГ®Г·ГЁГ©
use constant PROXY_STATUS_FAILED  => -1; # ГЏГ°Г®ГЄГ±ГЁ ГЇГ°Г®ГўГҐГ°ГҐГ­ ГЁ Г®Г­ Г°Г ГЎГ®Г·ГЁГ©


sub new {
    my $class = shift or die;
    my $self = bless $class->SUPER::new(@_), $class;
    $self->_proxies([]);
    return $self;
}

sub addProxy($$)
{
    my ($self, $proxy) = @_;
    push @{$self->_proxies}, { -ip=>$proxy, -status=>PROXY_STATUS_UNKNOWN };
}

sub getContent($$)
{
    my ($self, $url) = @_;
    my $ua = $self->userAgent;

    foreach my $proxy ( @{$self->_proxies} ) {
        next if $proxy->{-status} == PROXY_STATUS_FAILED;

        log_debug_f('Using proxy: %s [%s]', $proxy->{-ip},
            $proxy->{-status} == PROXY_STATUS_OK ? 'valid' : 'unchecked' );
        $ua->httpProxy( $proxy->{-ip} );
        my $content = $self->nextPlan->getContent($url);
        if( $content ) {
            $proxy->{-status} = PROXY_STATUS_OK;
            return $content;
        }
        $proxy->{-status} = PROXY_STATUS_FAILED;
    }

    return undef;
}

1;

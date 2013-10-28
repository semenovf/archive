package JQ::UserAgent;
#use Carp;
use File::Basename;
use File::Spec::Functions qw(catfile);
use File::Path;
use JQ::Logger;
use JQ::File::Unique;
use JQ::UserAgent::Plan;
use strict;
use warnings;

use base qw(LWP::UserAgent JQ::Errorable);

#use constant USERAGENT_NO_PLAN  => JQ::UserAgent::Plan::NoPlan->new;
use constant DEFAULT_ID         => 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.9.1.9) Gecko/20100315 Firefox/3.5.9';
use constant DEFAULT_TIMEOUT    => 3 * 60;  # 2 minuites
use constant DEFAULT_PROXY      => 'ENV';
#use constant DEFAULT_PLAN       => USERAGENT_NO_PLAN;
use constant DEFAULT_CACHE_ENC  => 'utf8';
use constant DEFAULT_CACHE_DIR  => './cache';

#my $userAgent = undef;

#sub instance
sub new
{
    my $class = shift or die;

    #return $userAgent if defined $userAgent;

    my $self = bless $class->SUPER::new(), $class;
    $self->agent(DEFAULT_ID);
    $self->timeout(DEFAULT_TIMEOUT);
    $self->httpProxy(DEFAULT_PROXY);
    $self->cacheEncoding(DEFAULT_CACHE_ENC);
    $self->cacheDir( DEFAULT_CACHE_DIR );
    $self->cacheOnDemand(1);
    $self->makeErrorable;
    $self->{-rootPlan} = JQ::UserAgent::Plan::Root->new;
    $self->{-rootPlan}->userAgent($self);
    $self->{-rootPlan}->nextPlan->userAgent($self); # set user agent for final plan

    #$userAgent = bless $self, __PACKAGE__;
    #return $userAgent;
    return $self;
}


sub cookie
{
    my $self = shift or die;
    my $cookie = shift;

    return $self->cookie_jar() unless defined $cookie;

    if( ref $cookie && ref $cookie eq 'HASH' && exists($cookie->{'file'}) ) {
        log_debug_f('Using cookie from %s', $cookie->{'file'});
    } else {
        log_debug('Using cookie');
    }
    $self->cookie_jar($cookie);
}


# Usage:
#   1. $ua->httpProxy('ENV')
#   2. $ua->httpProxy('DIRECT')
#   3. $ua->httpProxy('http://proxy.sn.no:8001/')
#
sub httpProxy($$)
{
    my $self = shift;
    my $proxy = shift;

    unless ( $proxy eq 'DIRECT' ) {
        if( $proxy eq 'ENV' ) {
            $self->env_proxy;
        } else {
            $self->proxy('http', $proxy);
        }
    }
}

sub addPlan($$)
{
    my ($self, $plan) = @_;
    $plan->userAgent($self);
    $self->{-rootPlan}->addPlan($plan);
    log_debug_f('Plan added: %s', ref $plan);
}


# Get/set cache directory
# Usage:
#   1. my $cdir = $ua->cacheDir
#   2. my $res = $ua->cacheDir($someDirectoryForCache);
#
sub cacheDir
{
    my $self = shift or die;
    my $dir = shift;

    return $self->{-cacheDir} unless defined $dir;

    unless ( -d $dir ) {
        my $err;
        File::Path::make_path($dir, { verbose=>1, error=>\$err} );

        if (@$err) {
            for my $diag (@$err) {
                my ($file, $msg) = %$diag;
                if ($file eq '') {
                    $self->setError($msg);
                }
            }
            return 0;
        }
    }
    $self->{-cacheDir} = $dir;
    return 1;
}

 #Usage
 #  1. my $isCachOnDemand = $ua->cacheOnDemand;
 #  2. $ua->cacheOnDemand( $boolean );
sub cacheOnDemand
{
    my $self = shift or die;
    my $what = shift;

    return $self->{-cacheOnDemand} unless defined $what;

    $self->{-cacheOnDemand} = $what ? 1 : 0;
    return $self->{-cacheOnDemand};
}


# Usage
#   1. $ua->cacheEncoding( $enc );
sub cacheEncoding
{
    my $self = shift or die;
    my $enc = shift;

    return $self->{-cacheEncoding} unless defined $enc;

    $self->{-cacheEncoding} = $enc;
    return $self->{-cacheEncoding};
}


# Get content
# Usage:
#   1. $ua->get($url)
sub getContent($$)
{
    return $_[0]->{'-rootPlan'}->getContent($_[1]);
}

#
# for internal use only
#
sub _getContent($$)
{
    my ($self, $url) = @_;

    unless( ref $url && ref $url eq 'URI' ) {
        $url = URI->new($url);
    }

    log_debug_f( "Getting: %s", $url->canonical );

    my $req = HTTP::Request->new(GET => $url->canonical);
    my $res = $self->request($req);

    if ($res->is_success) {
        my $content = $res->decoded_content;
        $self->cacheContent($content) unless $self->cacheOnDemand;
        return $content;
    }

    $self->setError( $res->status_line );
    return undef;
}


# Saving content in cache
# Usage:
#   $self->cacheContent($content)
sub cacheContent($$)
{
    my( $self, $content ) = @_;

    my $path = uniqueFileName($self->{-cacheDir}, 'jq_', '.html');
    return 0 unless $path;

    open( FH, '>', $path ) or do {
        $self->setError( "Unable to create file for caching content: %s: %s", $path, $! );
#        log_error_f( "Unable to create file for caching content: %s: %s", $path, $! );
        return 0;
    };
    binmode( FH, ':encoding('.$self->{-cacheEncoding}.')' );
    print FH $content;
    close( FH );

    log_debug_f("Content cached at %s", $path);
    return 1;
}

1;

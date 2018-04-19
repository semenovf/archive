package JQ::Web::Quest::Expert::Iterator;
use URI;
use JQ::Logger;
use JQ::UserAgent;
use strict;
use warnings;

use base 'Class::Accessor';
__PACKAGE__->mk_accessors(qw(
    expert
    pages
    _htmlTree
    _uri
));


sub new
{
    my ($class, $expert, %args) = @_;
    my $content = $expert->getContent($expert->url);
    unless( $content ) {
        log_debug_f("%s: empty or undef content was obtained from '%s'", $expert->name, $expert->url);
        return undef;
    }
    my $self = bless $class->SUPER::new(), $class;
    $expert->content($content);
    $self->expert($expert);
    $self->url($expert->url);
    $self->pages( 1 );
    return $self;
}


sub asHtmlTree
{
    my $self = shift or die;

    unless( $self->_htmlTree ) {
        $self->_htmlTree( $self->expert->buildHtmlTree() );
    }

    return $self->_htmlTree;
}

sub _deleteHtmlTree
{
    if( $_[0]->_htmlTree ) {
        $_[0]->_htmlTree->delete;
        $_[0]->_htmlTree(undef);
    }
}


sub next($)
{
    my $self = $_[0];

    unless( $self->pages < $self->expert->pagesLimit ) { # avoid unnecessary request
        log_debug_f("%s: limit by pages [%s] has been exceeded", $self->expert->name, $self->expert->pagesLimit);
        $self->expert->content('');
        return 0;
    }

    my $href = $self->expert->nextPageHRef($self->asHtmlTree);
    return 0 unless( $href );
    #{
    #    log_debug_f("%s: href for next page at [%s] is undefined", $self->expert->name, $self->url);
    #    return 0;
    #}

    $self->url($href);
    my $content = $self->expert->getContent($self->url);

    unless( $content ) {
        log_debug_f("%s: empty or undef content was obtained from '%s'", $self->expert->name, $self->url);
        return 0;
    }
    $self->_deleteHtmlTree();
    $self->expert->content($content);
    $self->pages( $self->pages+1 );
    return 1;
}

sub delete($)
{
    $_[0]->_deleteHtmlTree();
}

sub url($;)
{
    my $self = shift;
    my $href = shift;

    return $self->_uri->as_string unless defined $href;

    $self->_uri( URI->new_abs($href, $self->expert->url) );
}

#sub hasMore($)
#{
#    my $self = $_[0];
#    unless( $self->expert->content ) { $self->_deleteHtmlTree(); return 0; }
#    unless( $self->pages <= $self->expert->pagesLimit ) {
#        log_debug_f("%s: limit by pages [%s] has been exceeded", $self->expert->name, $self->expert->pagesLimit);
#        $self->_deleteHtmlTree();
#        return 0;
#    }
#
#    my $href = $self->expert->nextPageHRef($self->asHtmlTree);
#    $self->url($href) if $href;
#
#    return 1 if $self->pages == 1;
#    unless( $href ) { $self->_deleteHtmlTree(); return 0; }
#    return 1;
#}

1;

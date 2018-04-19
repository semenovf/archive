package JQ::Web::Quest::Expert;
use Encode;
use HTML::TreeBuilder::XPath;
use JQ::Web::Quest::Expert::Iterator;
#use JQ::Logger;
use strict;
use warnings;

use base 'Class::Accessor';
__PACKAGE__->mk_accessors(qw(
    banned
    inconsistent
    content
    name
    url
    urlTemplate
    quest
    _pagesLimit
    _daysLimit
    _hook
    _hookArgs
));

#    iterator

sub MAX_PAGES_LIMIT {1000}
sub MAX_DAYS_LIMIT {365}


sub new
{
    my $class = shift or die;
    my $self = bless $class->SUPER::new(), $class;
    $self->banned(0);
    $self->inconsistent(0);
    $self->name('<undef>');
    $self->_pagesLimit(1);
    #$self->_hook(sub {});
    $self->_hookArgs([]);
    return $self;
}

sub urlSubst
{
    my $self = shift or die;
    my $subst_hash = shift;

    if( $subst_hash ) {
        my $url_tmpl = $self->urlTemplate;
        while( my ($k, $substr) = each( %{$subst_hash} ) ) {
            $url_tmpl =~ s/\[%$k%\]/$substr/g;
        }
        $self->url($url_tmpl);
    } else {
        $self->url($self->urlTemplate);
    }
}

sub setHook
{
    my $self = shift or die;
    my $hook = shift;
    my @hook_args = @_;

    if( defined $hook ) {
        if( ref $hook && ref $hook eq 'CODE' ) {
            $self->_hook($hook);
            $self->_hookArgs(\@hook_args);
        }
    }
}

sub callback
{
    my $self = shift or die;
    return $self->_hook->(-Expert=>$self, @_, @{$self->_hookArgs}) if defined $self->_hook;
}

sub getContent($$)
{
    return $_[0]->quest->userAgent->getContent($_[1]);
}

sub pagesLimit
{
    my $self = shift or die;
    my $limit = shift;

    return $self->_pagesLimit unless defined $limit;
    if( $limit < 1 || $limit > MAX_PAGES_LIMIT ) {
        $limit = MAX_PAGES_LIMIT;
    }
    $self->_pagesLimit($limit);
}

sub daysLimit
{
    my $self = shift or die;
    my $limit = shift;

    return $self->_daysLimit unless defined $limit;
    if( $limit < 1 || $limit > MAX_DAYS_LIMIT ) {
        $limit = MAX_DAYS_LIMIT;
    }
    $self->_daysLimit($limit);
}

sub buildHtmlTree
{
    my $self = shift or die;
    my $content = shift;
    $content = $self->content unless $content;
    return undef unless $content;

    my $t = HTML::TreeBuilder::XPath->new;
    $t->implicit_tags(1);        #
    $t->no_space_compacting(0);
    $t->store_comments(0);	     #
    $t->store_declarations(0);	 #
    $t->ignore_unknown(0);	     #
    $t->warn(1); 		         #

    $t->parse( decode_utf8($content) );
    $t->eof();
    return $t;
}

sub begin
{
    my $self = shift;
    my %args = @_;
    my $it = JQ::Web::Quest::Expert::Iterator->new($self, %args);
    return $it;
}

sub nextPageHRef {undef}
sub parsePage    {0}

1;

#
# Copyright (C) 2010,2011 Vladislav Trifochkin
# MetaPage is a part of Met@Page software
# Met@Page is free software; you can redistribute it and/or
# modify it under the same terms as Perl itself.
#

package MetaPage;
use Carp;
use MetaPage::Parser;
use MetaPage::HandlerFactory::Plain;
use strict;
use warnings;

my $_post_render = sub {$_[0]};

#
#@TODO: need modification for implement dynamic post renderer
# by definition in form:
# use MetaPage qw(Text::Simplify)
#
sub import
{
    require Text::Simplify;
    $_post_render = \&Text::Simplify::simplify;
}

sub new
{
    my $class = shift;
    return bless {}, (ref $class ? ref $class : $class);
}

sub var
{
    my ($self, $name, $val) = @_;
    $self->{'vars'} = {} unless defined $self->{'vars'};
    
    return (exists $self->{'vars'}->{$name} ? $self->{'vars'}->{$name} : '')
        unless defined $val;
    $self->{'vars'}->{$name} = $val;
    return $self;
}

#
# set bulk of vars
#
sub set_vars
{
    my $self = shift;
    
    return $self unless @_ > 0;
    
    my $vars;
    if( @_ > 2 ) {
        my %vars = @_ if @_ > 2;
        $vars = \%vars;
    } else {
        $vars = $_[0];
    }
    
    $self->{'vars'} = {} unless defined $self->{'vars'};

    while( my ($name,$val) = each %$vars ) {
        $self->{'vars'}->{$name} = $val;
    }
    return $self;
}

sub render
{
    my ($self, $data, $vars) = @_;
    my $parser = MetaPage::Parser->new(ErrorContext=>4);
    $parser->media('html');
    $parser->metapage($self);
    MetaPage::HandlerFactory::Plain->handlersFor($parser);

    $self->set_vars($vars) if $vars && ref $vars && ref $vars eq 'HASH';
    
    if( ref $data ) {
        $parser->parse_text($data);
    } else {
        -f $data or croak sprintf('File not found: %s', $data);
        $parser->parse_file($data);
    }
    $parser->release;
    return $_post_render->($parser->render);
}

1;

__END__

=head1

    use MetaPage;
    
    my $mp = MetaPage->new;

    my $content = $mp->render('path/to/template');
    -or-
    my $content = $mp->render(\$text);

=cut
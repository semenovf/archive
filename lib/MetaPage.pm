#
# Copyright (C) 2010,2011 Vladislav Trifochkin
# MetaPage is a part of Met@Page software
# Met@Page is free software; you can redistribute it and/or
# modify it under the same terms as Perl itself.
#

package MetaPage;
use Carp;
use MetaPage::HandlerFactory::Plain;
use MetaPage::Const;
use base 'XML::Parser::Expat';
use strict;
use warnings;


sub _is_root_elem # [protected]
{
    my ($self, $elem) = @_;
    return (lc $elem eq _ROOT_);
}

sub _append # [protected]
{
    my ($self, $what, @data) = @_;
    $self->{$what} = [] unless defined $self->{$what};
    push @{$self->{$what}}, @data;
}

sub _aliases # [protected]
{
    my $self = shift;
    $self->{&_ALIASES_} = {} unless defined $self->{&_ALIASES_};
    return $self->{&_ALIASES_};
}

sub media
{
    my ($self, $media) = @_;
    $media = 'html' unless defined $media;
    $self->{'.media'} = 'html' unless defined $self->{'.media'};
    return $self->{'.media'} if( @_ < 2 );
    $self->{'.media'} = $media;
    return $self;
}

sub parse_text
{
    return $_[0]->parse(join '', @_);
}


sub parse_file
{
    my ($self, $path) = @_;
    
    open( my $fh, '<', $path ) or croak sprintf('%s: %s', $path, $!);
    local $/;
    $self->parse(<$fh>);
    close $fh;
}


sub render_code
{
    join("\n", @{$_[0]->{&_INCLUDES_}}, @{$_[0]->{&_TEXT_}});
}


sub render
{
    eval join("\n", @{$_[0]->{&_INCLUDES_}}, @{$_[0]->{&_TEXT_}});
}


1;

__END__

=head1

use MetaPage;
use MetaPage::HandlerFactory;

my $mp = MetaPage->new;
MetaPage::HandlerFactory->handlersFor($mp);

or

my $mp = MetaPage::HandlerFactory->handlersFor(MetaPage->new);

...

local $/;
open my $fh, '<', '/path/to/file' or die;
$mp->parse_text(<$fh>);
close $fh;

or

$mp->parse_file('/path/to/file');

...

$mp->render_code; # render intermediate perl-code
$mp->render;      # deep render

=cut
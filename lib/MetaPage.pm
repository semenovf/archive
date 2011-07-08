#
# Copyright (C) 2010,2011 Vladislav Trifochkin
# MetaPage is a part of Met@Page software
# Met@Page is free software; you can redistribute it and/or
# modify it under the same terms as Perl itself.
#

package MetaPage;
use Carp;
use base qw(XML::Parser::Expat);

use strict;
use warnings;

sub _INCLUDES_  {'.includes'}
sub _TEXT_      {'.text'}
sub _ALIASES_   {'.aliases'}

sub ROOT      {'metapage'}

sub parse_text
{
    shift->SUPER::parse(join '', @_);
}


sub parse_file
{
    my ($self, $path) = @_;
    
    open my $fh, '>', $path or croak sprintf('%s: %s', $path, $!);
    local $/;
    $self->SUPER::parse(<$fh>);
    close $fh;
}

sub _is_root_elem # [protected]
{
    my ($self, $elem) = @_;
    return (lc $elem eq ROOT);
}

sub _append # [protected]
{
    my ($self, $what, @data) = @_;
    $self->{$what} = [] unless defined $self->{$what};
    push @{$self->{$what}}, @data;
}

sub _aliases # [protected]
{
    my $aliases = &_ALIASES_;
    $_[0]->{$aliases} = {} unless defined $_[0]->{$aliases};
    return $_[0]->{$aliases};
}

sub render
{
    my $self = $_[0];
    foreach( @{$self->{&_INCLUDES_}}, @{$self->{&_TEXT_}} ) {
        print $_, "\n"; 
    }
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

$mp->render;

=cut
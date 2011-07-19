#
# Copyright (C) 2010,2011 Vladislav Trifochkin
# MetaPage is a part of Met@Page software
# Met@Page is free software; you can redistribute it and/or
# modify it under the same terms as Perl itself.
#

package MetaPage::Parser;
use Carp;
#use Class::Accessor;
use base qw(XML::Parser::Expat Class::Accessor);
use strict;
use warnings;

sub _INCLUDES_  {'.includes'}
sub _TEXT_      {'.text'}
sub _ALIASES_   {'.aliases'}


__PACKAGE__->mk_accessors(qw(
    metapage
    media
    unit
    root
));


sub release
{
    my $self = shift;
    $self->SUPER::release;
    $self->{&_ALIASES_}  = undef;
    $self->{&_INCLUDES_} = undef;
    $self->{&_TEXT_} = undef;
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

# $mp->parse_text
# $mp->parse_text( \$text )
# $mp->parse_text( @lines )
#
sub parse_text
{
    my $self = shift;
    return '' if @_ == 0;
    return $self->parse( ${$_[0]} ) if @_ == 1 && ref $_[0];
    return $self->parse(join '', @_);
}


sub parse_file
{
    my ($self, $path) = @_;
    
    open( my $fh, '<', $path ) or croak sprintf('%s: %s', $path, $!);
    local $/;
    my $content = <$fh>;
    $self->parse_text(\$content);
    close $fh;
}


sub render_il_code
{
    my $self = $_[0];
    my $code = join("\n",
         @{$_[0]->{&_INCLUDES_}},
         @{$_[0]->{&_TEXT_}}
    );
    return $code;
}

1;

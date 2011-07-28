package Webject;
use Webject::Media::Functions qw(!media);
use Carp;
use Text::Trim;
use Webject::Clipboard;
use base 'Webject::Accessor';

use 5.006;
use strict;
use warnings;

=head1 NAME

Webject (Web [Ob]ject) - base class for Web[Ob]jects 

=head1 VERSION

Version 0.02

=cut

our $VERSION = '0.02';

my $_CLIPBOARDS = {};

__PACKAGE__->mk_accessors(qw(mode));
#
# Modes
#
sub MODE_VIEW {'view'}
sub MODE_EDIT {'edit'}

=head1 SYNOPSIS

Base class for web objects (Webjects).

    use Webject;

    my $webj = Webject->new();
    $webj->add( Webject::Foo->new );
    $webj->add( Webject::Bar->new );
    ...
    $webj->render;

=head1 SUBROUTINES/METHODS

=head2 new

=cut


sub new
{
    my $class = shift;
    my $self = bless {
        -parent => undef,       # parent webject reference
        -children => [],
        -atts => []
    }, $class;
    $self->ctor;
    return $self;
}

sub ctor
{
    my $self = shift;
    $self->set_attributes(qw(
        id
        class
        style
    ));
    $self->mode(MODE_VIEW);
}

sub media
{
    return Webject::Media->media;
}

#sub clear_attributes
#{
#    $_[0]->{-atts} = [];
#}

sub set_attributes
{
    my ($self, @atts) = @_;
    ref($self)->mk_accessors(@atts);
    push @{$self->{-atts}}, @atts;
    return $self;
}


=head2 add

=cut

sub add
{
    my $self = shift or die;
    my @what = @_;

    foreach( @what ) {
        if( ref $_ && $_->isa(__PACKAGE__) ) {
            if( defined $_->{-parent} ) {
                carp sprintf(q(Unable to add webject(type=%s, id=%s) as child to webject(type=%s, id=%s): it is already a child for webject(type=%s, id=%s)),
                    ref $_, defined $_->id ? $_->id : '<unset>',
                    ref $self, defined $self->id ? $self->id : '<unset>',
                    ref ${$_->{-parent}}, defined ${$_->{-parent}}->id ? ${$_->{-parent}}->id : '<unset>');
                next;
            }
        } else {
            require Webject::Text;
            $_ = Webject::Text->new->value($_);
        }
        $_->{-parent} = \$self;
        push @{$self->{-children}}, $_;
    }
    return $self;
}


=head2 render_atts

=cut

sub render_atts
{
    my $self = shift;
    return stringify_atts($self->{-atts}, $self);
}


=head2 render

=cut

sub render
{
    my $self = shift;
    return $self->_render_text( $self->_render_source );
}


# _render_source
sub _render_source
{
    my $self = shift;
    
    my $is_a = '@'. ref($self) .'::ISA';
    my @bases = eval "$is_a";
    my $data_fh;
    
    if( ref($self) eq __PACKAGE__) {
        $data_fh = __PACKAGE__.'::DATA';
        $data_fh  = eval "*$data_fh";
        confess $@ if $@;
    } else {
        for my $pkg ( (ref($self), @bases) ) {
            last if $pkg eq __PACKAGE__;
            
            $data_fh = $pkg.'::DATA';
            #eval "require $pkg";
            $data_fh  = eval "*$data_fh";
            confess $@ if $@;
            
            last if defined fileno($data_fh);
            undef $data_fh;
        }
    }
    
    croak 'no source found for render' unless defined $data_fh;  # no DATA section found

    local $/;
    my $pos = tell $data_fh;
    $_ = <$data_fh>;
    seek $data_fh, $pos, 0;
    return $_;
}


=head2 render_text [protected method]

=cut

sub _render_text
{
    my $self = shift;
    my $m = Webject::Media->media;
    
    $_ = join('', map { defined $_ ? $_ : '' } @_);
    $_ =~ /<!--${m}\{\s*(.*?)\}${m}-->/sg and do { $_ = $1 };
    my @text = split(/(<%=?\s*.*?)\s*%>/s, $_);

    my @code = ();
    foreach( @text ) {
        if( /^<%=(.+)/s ) {
            $_ = $1;
            push @code, "push \@__text__, (&{sub{ $_ }} || '');\n";
        } elsif( /^<%(.+)/s ) {
            push @code, $1;
        } else {
            push @code, "push \@__text__, q($_);\n";
        }
    }
    #print join("\n", 'my @__text__;', @code, q(join('', @__text__);));
    
    my $text = eval( join("\n", 'my @__text__;', @code, q(join('', @__text__);)) ) || '';
    confess $@ if $@;
    
    #$self->clear_clipboards;
    return $text;
}


=head2 render_file [protected method]

=cut

#sub render_file
#{
#    my ($self, $fh) = @_;
#    local $/;
#    my $pos = tell $fh;
#    $_ = <$fh>;
#    seek $fh, $pos, 0;
#    return $self->render_text($_);
#}




=head2 render_children [protected method]

=cut

sub render_children
{
    my $self = $_[0];
    my $text = '';

    foreach ( @{$self->{-children}} ) {
        if( ref $_ && $_->isa(__PACKAGE__) ) {
            $text .= $_->render();
        } else {
            carp "Invalid child, ignored";
        }
    }
    return $text;
}


sub _children
{
    return wantarray ? @{$_[0]->{-children}} : $_[0]->{-children};
}


sub children_count
{
    return scalar @{$_[0]->{-children}};
}

sub child_at
{
    my( $self, $index) = @_;
    return undef unless defined $index;
    return undef if( $index < 0 || $index >= $self->children_count);
    return $self->{-children}->[$index];
}

sub push_clipboard
{
    my $self = shift;
    my $name = shift;
    
    $_CLIPBOARDS->{$name} = Webject::Clipboard->new unless exists $_CLIPBOARDS->{$name};
    $_CLIPBOARDS->{$name}->push(@_);
}

sub fetch_clipboard
{
    my $self = shift;
    my $name = shift;
    return $_CLIPBOARDS->{$name} ? $_CLIPBOARDS->{$name}->fetch : '';
}

sub clear_clipboard
{
    my $self = shift;
    my $name = shift;
    return unless exists $_CLIPBOARDS->{$name};
    $_CLIPBOARDS->{$name}->clear;
    undef $_CLIPBOARDS->{$name};
}


sub clear_clipboards
{
    my $self = shift;
    for my $name ( keys %{$_CLIPBOARDS} ) {
        $_CLIPBOARDS->{$name}->clear;
    }
    $_CLIPBOARDS = {};
}

=head1 AUTHOR

Fedor Semenov, C<< <fedor.v.semenov at gmail.com> >>

=head1 BUGS

Please report any bugs or feature requests to C<bug-web-webject at rt.cpan.org>, or through
the web interface at L<http://rt.cpan.org/NoAuth/ReportBug.html?Queue=Web-Webject>.  I will be notified, and then you'll
automatically be notified of progress on your bug as I make changes.

=head1 SUPPORT

You can find documentation for this module with the perldoc command.

    perldoc Webject


You can also look for information at:

=over 4

=item * RT: CPAN's request tracker (report bugs here)

L<http://rt.cpan.org/NoAuth/Bugs.html?Dist=Webject>

=item * AnnoCPAN: Annotated CPAN documentation

L<http://annocpan.org/dist/Webject>

=item * CPAN Ratings

L<http://cpanratings.perl.org/d/Webject>

=item * Search CPAN

L<http://search.cpan.org/dist/Webject/>

=back


=head1 ACKNOWLEDGEMENTS


=head1 LICENSE AND COPYRIGHT

Copyright 2011 Fedor Semenov.

This program is free software; you can redistribute it and/or modify it
under the terms of either: the GNU General Public License as published
by the Free Software Foundation; or the Artistic License.

See http://dev.perl.org/licenses/ for more information.


=cut

1; # End of Webject

__DATA__

<!--html{
<%=$self->render_children %>
}html-->

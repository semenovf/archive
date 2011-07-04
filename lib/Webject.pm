package Webject;
use Webject::Media::Functions;
use Carp;
use base 'Webject::Accessor';

use 5.006;
use strict;
use warnings;

=head1 NAME

Webject (Web [Ob]ject) - base class for Web[Ob]jects 

=head1 VERSION

Version 0.01

=cut

my @rw_atts = qw(
    id
    class
);

__PACKAGE__->mk_accessors(@rw_atts);

our $VERSION = '0.01';

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

sub new {
    my $class = shift;
    my $self = bless {
        -parent => undef,       # parent webject reference
        -children => []
    }, $class;
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


=head2 render

=cut

sub render
{
    my $self = $_[0];
    my $data_fh = ref($self).'::DATA';
    $data_fh  = eval "*$data_fh";
    croak $@ if $@;
    
    return $self->render_file( $data_fh );
}


=head2 render_atts [protected method]

=cut

sub render_atts
{
    my $self = shift;
    my %atts = ();
    foreach( @rw_atts ) {
        $atts{$_}=$self->$_ if defined $self->$_;
    } ;
    return stringify_atts(%atts);
}


=head2 render_text [protected method]

=cut

sub render_text
{
    my $self = shift;
    my $m = Webject::Media->media;
    $_ = join('', @_);
    $_ =~ /<!--${m}\{\s*(.*?)\}${m}-->/sg and do { $_ = $1 };
    my @text = split(/(<%=?\s*.*?)\s*%>/s, $_);

    my @code = ();
    foreach( @text ) {
        if( /^<%=(.+)/s ) {
            push @code, "push \@__text__, (&{sub{ $1 }} || '');\n";
        } elsif( /^<%(.+)/s ) {
            push @code, $1;
        } else {
            push @code, "push \@__text__, q($_);\n";
        }
    }
#    print join("\n", 'my @__text__;', @code, q(join('', @__text__);));
    my $text = eval( join("\n", 'my @__text__;', @code, q(join('', @__text__);)) ) || '';
    croak $@ if $@;
    return $text;
}


=head2 render_file [protected method]

=cut

sub render_file
{
    my ($self, $fh) = @_;
    local $/;
    my $pos = tell $fh;
    $_ = <$fh>;
    seek $fh, $pos, 0;
    return $self->render_text($_);
}




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


sub _children_count
{
    return scalar @{$_[0]->{-children}};
}

sub _child_at
{
    my( $self, $index) = @_;
    return undef unless defined $index;
    return undef if( $index < 0 || $index >= $self->_children_count);
    return $self->{-children}->[$index];
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

package Webject;
use base 'Class::Accessor';

use 5.006;
use strict;
use warnings;

=head1 NAME

Webject (Web [Ob]ject) - web objects 

=head1 VERSION

Version 0.01

=cut

our $VERSION = '0.01';

=head1 SYNOPSIS

Base class for web objects (Webjects).

    use Webject;

    my $webj = Webject->new();
    ...

=head1 SUBROUTINES/METHODS

=head2 new

=cut

sub new {
    my $class = shift;
#    my %attrs = @_;
    my $self = bless {
        -children => []
    }, $class;
    #$self->ctor();
    #$self->units('px');
    #if( %attrs ) {
    #    $self->id( $attrs{'id'} );
    #    $self->class( $attrs{'class'} );
    #    $self->style( $attrs{'style'} );
    #}
    return $self;
}

=head2 add

=cut

sub add
{
    my $self = shift or die;
    my @what = @_;

    foreach( @what ) {
        $self->{-children} = () unless defined $self->{-children};
        push @{$self->{-children}}, $_;
    }
    return $self;
}


=head2 render

=cut

sub render
{
    return $_[0]->render_file(*DATA);
}


sub render_file
{
    my ($self, $fh) = @_;
    local $/;
    my $pos = tell $fh;
    $_ = '\''.<$fh>.'\'';
    $_ =~ s/<%(.*?)%>/\'\.\($1\)\.\'/sg;
    seek $fh, $pos, 0;
    return eval $_;
}


# 
sub render_children
{
    my $self = $_[0];
    my $html = '';

    foreach ( @{$self->{-children}} ) {
        if( ref $_ && $_->isa(__PACKAGE__) ) {
            $html .= $_->render();
        } else {
            $html .= $_;
        }
    }
    return $html;
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

L<http://cpanratings.perl.org/d/Web-Webject>

=item * Search CPAN

L<http://search.cpan.org/dist/Web-Webject/>

=back


=head1 ACKNOWLEDGEMENTS


=head1 LICENSE AND COPYRIGHT

Copyright 2011 Fedor Semenov.

This program is free software; you can redistribute it and/or modify it
under the terms of either: the GNU General Public License as published
by the Free Software Foundation; or the Artistic License.

See http://dev.perl.org/licenses/ for more information.


=cut

1; # End of Web::Webject

__DATA__
<% $self->render_children %>
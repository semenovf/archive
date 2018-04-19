package Dancer::Template::MetaPage;
use Carp;
use Dancer::ModuleLoader;
use base 'Dancer::Template::Abstract';
use strict;
use warnings;

=head1 NAME

Dancer::Template::MetaPage - MetaPage wrapper for Dancer.

=head1 VERSION

Version 0.01

=cut

our $VERSION = '0.01';


=head1 SYNOPSIS

    use Dancer::Template::MetaPage;

=cut

my $_engine;

sub default_tmpl_ext { 'mp.xml' }

sub init
{
    my $self = shift;
    my $settings = $self->config;
    my $class = 'MetaPage';

    my ($rc, $emsg) = Dancer::ModuleLoader->load($class);
    $rc or croak sprintf( q(Loading '%s' class for 'Dancer::Template::MetaPage' failed: %s),
        $class, $emsg);

    $_engine = $class->new;

    #if( (my $dump_code = $self->config->{'dump_code'}) ) {
    #    if( $dump_code eq 'STDOUT' ) {
    #        $_engine->dump_code(*STDOUT);
    #    } elsif( $dump_code eq 'STDERR' ) {
    #        $_engine->dump_code(*STDERR);
    #    } else {
    #        $_engine->dump_code(*STDOUT);
    #    }
    #}
}

#sub layout
#{
#    my ($self, $layout, $tokens, $content) = @_;
#    $_engine->set_vars(%$tokens, 'content'=>$content);
#}

sub render
{
    my ($self, $template, $tokens) = @_;

    if ( ! ref $template ) {
        -f $template or croak sprintf(q('%s' doesn't exist or not a regular file), $template);
        return $_engine->render($template, $tokens);
    }

    return $_engine->render($template, $tokens);
}



=head1 AUTHOR

Fedor Semenov, C<< <fedor.v.semenov at gmail.com> >>

=head1 BUGS

Please report any bugs or feature requests to C<bug-dancer-template-metapage at rt.cpan.org>, or through
the web interface at L<http://rt.cpan.org/NoAuth/ReportBug.html?Queue=Dancer-Template-MetaPage>.
I will be notified, and then you'll automatically be notified of progress on your bug as I make changes.

=head1 SUPPORT

You can find documentation for this module with the perldoc command.

    perldoc Dancer::Template::MetaPage

You can also look for information at:

=over 4

=item * RT: CPAN's request tracker (report bugs here)

L<http://rt.cpan.org/NoAuth/Bugs.html?Dist=Dancer-Template-MetaPage>

=item * AnnoCPAN: Annotated CPAN documentation

L<http://annocpan.org/dist/Dancer-Template-MetaPage>

=item * CPAN Ratings

L<http://cpanratings.perl.org/d/Dancer-Template-MetaPage>

=item * Search CPAN

L<http://search.cpan.org/dist/Dancer-Template-MetaPage/>

=back


=head1 ACKNOWLEDGEMENTS


=head1 LICENSE AND COPYRIGHT

Copyright 2011 Fedor Semenov.

This program is free software; you can redistribute it and/or modify it
under the terms of either: the GNU General Public License as published
by the Free Software Foundation; or the Artistic License.

See http://dev.perl.org/licenses/ for more information.


=cut

1; # End of Dancer::Template::MetaPage

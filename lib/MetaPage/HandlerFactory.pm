#
# Copyright (C) 2010,2011 Vladislav Trifochkin
# MetaPage::HandlerFactory is a part of Met@Page software
# Met@Page is free software; you can redistribute it and/or
# modify it under the same terms as Perl itself.
#
package MetaPage::HandlerFactory;
use Carp;
use MetaPage;
#use Text::Simplify;

use strict;
use warnings;

=head1
    
    MetaPage::HandlerFactory->handlersFor( $parser )
    
=cut

my %_handlers = (
    Start        => \&_on_start_elem,
    End          => \&_on_end_elem,
    Char         => \&_on_text,
    Comment      => \&_on_comment
);

my %_mp_handlers = (
    'use' => \&_on_mp_use
);


sub handlersFor {
    my ($class, $parser) = @_;
    $parser->setHandlers ( %_handlers );
    return $parser;
}

sub _on_start_elem # (Expat, Element [, Attr, Val [,...]])
{
    my ($parser, $elem, %atts) = @_;

    if( $elem =~ /^mp:(.+)$/ ) {
        my $mpt = $1;
        
        if( exists $_mp_handlers{$mpt} ) {
            $_mp_handlers{$mpt}->($parser, \%atts);
        } else {
            
        }
    } else {
        $parser->_append(MetaPage::TEXT, "<$elem>"); # TODO: need to add attributes
    }
}

sub _on_end_elem # (Expat, Element)
{
    my ($parser, $elem) = @_;
    if( $elem =~ /^mp:(.+)$/ ) {
        my $mpt = $1;
    } else {
        $parser->_append(MetaPage::TEXT, "</$elem>");
    }
}

sub _on_text # (Expat, String)
{
    my ($parser, $text) = @_;
    #simplify($text);
    $parser->_append(MetaPage::TEXT, $text);
}

sub _on_comment # (Expat, Data)
{
    my ($parser, $data) = @_;
    $parser->_append(MetaPage::TEXT, $data);
}


sub _on_mp_use # (parser, HASHREF)
{
    my ($parser, $atts) = @_;
    my $webject = $atts->{'webject'};
    my $as = $atts->{'as'};
    
    while(1) {
        last if $webject;
        last if $as;
        croak q(incomplete 'use' tag, use like this: <mp:use webject="SomeWebjectClass" as="Alias" />);
        return;
    }
    
    $parser->_append(MetaPage::PREFIX, "use $webject;\n");
}

1;

#
# Copyright (C) 2010,2011 Vladislav Trifochkin
# MetaPage::HandlerFactory is a part of Met@Page software
# Met@Page is free software; you can redistribute it and/or
# modify it under the same terms as Perl itself.
#
package MetaPage::HandlerFactory::Plain;
use Carp;
use MetaPage;
use Text::Simplify;

use strict;
use warnings;

=head1
    
    MetaPage::HandlerFactory->handlersFor( $parser )
    
=cut

my %_handlers = (
    Start        => \&_on_start_elem,
    End          => \&_on_end_elem,
    Char         => \&_on_text,
);

my %_mp_handlers = (
    'use' => \&_on_mp_use
);


sub handlersFor {
    my ($class, $parser) = @_;
    $parser->setHandlers ( %_handlers );
    return $parser;
}


# replace Webejct::Media::html::stringify_atts whith function
# _stringify_atts(HASHREF)
sub _stringify_atts
{
    my @pairs = ();
    while( my ($k,$v) = each(%{$_[0]}) ) {
        push @pairs, "$k=\"$v\"";
    }
    return join( ' ', @pairs);
}

sub _stringify_args
{
    my @args = ();
    foreach( @_ ) {
        push @args, '\''. quotemeta($_) .'\'';
    }
    return join( ',', @args);
}


sub _on_start_elem # (Expat, Element [, Attr, Val [,...]])
{
    my ($parser, $elem, %atts) = @_;
    
    # root elem
    if( $parser->_is_root_elem($elem) ) {
        $parser->{'webject_counter'} = 0;
        $parser->{'parents'} = []; # stack of parents 
        
        $parser->_append(MetaPage::_INCLUDES_,
            'use Webject;',
        );
        $parser->_append(MetaPage::_TEXT_, 
            '{',
            'local $_;',
            '$_ = Webject->new();'
        );
        
        push @{$parser->{'parents'}}, '$_';

    } elsif ( $elem =~ /^mp:(.+)$/ ) {
        my $mpt = $1;
        
        if( exists $_mp_handlers{$mpt} ) {
            $_mp_handlers{$mpt}->($parser, 1, \%atts);
        } else {
            my $class = $parser->_aliases->{$mpt};
            
            croak sprintf( q(%s: unknown alias, use 'use' tag for define it!), $mpt )
                unless $class;
            
            # Variable name for webject instance
            my $vname = sprintf('$_%d_%s', ++$parser->{'webject_counter'}, $mpt );
            
            if( %atts ) {
                $parser->_append(MetaPage::_TEXT_, 
                    sprintf('my %s = %s->new()->atts(%s);', $vname, $class, _stringify_args(%atts)));
            } else {
                $parser->_append(MetaPage::_TEXT_, sprintf('my %s = %s->new();', $vname, $class));
            }
            my $parent = $parser->{'parents'}->[scalar(@{$parser->{'parents'}})-1];
            $parser->_append(MetaPage::_TEXT_, sprintf('%s->add(%s);', $parent, $vname));
            push @{$parser->{'parents'}}, $vname;
        }
    } else {
        my $mpt = 'Text';
        my $class = $parser->_aliases->{$mpt};
        croak sprintf( q(%s: unknown alias, use 'use' tag for define it!), $mpt )
            unless $class;
        
        my $vname = sprintf('$_%d_Text', ++$parser->{'webject_counter'} );
            
        if( %atts ) {
            $parser->_append(MetaPage::_TEXT_, 
                sprintf('my %s = %s->new()->value(\'<%s %s>\');',
                    $vname, $class, $elem, _stringify_atts(\%atts)));
        } else {
            $parser->_append(MetaPage::_TEXT_, 
                sprintf('my %s = %s->new()->value(\'<%s>\');',
                    $vname, $class, $elem));
        }

        push @{$parser->{'parents'}}, $vname;
    }
}

sub _on_end_elem # (Expat, Element)
{
    my ($parser, $elem) = @_;
    if( $parser->_is_root_elem($elem) ) {
        $parser->_append(MetaPage::_TEXT_,
            '$_->render;',
            '}'
        );
    } else {
        pop @{$parser->{'parents'}};
    }
}

sub _on_text # (Expat, String)
{
    my ($parser, $text) = @_;
    simplify($text);
    if( $text ) { # text is not empty
        my $vname = sprintf('$_%d_Text', ++$parser->{'webject_counter'} );
        my $class = $parser->_aliases->{'Text'};
        $parser->_append(MetaPage::_TEXT_, 
            sprintf('my %s = %s->new()->value(\'%s\');',
                $vname, $class, $text));
    }
}

sub _on_mp_use # (parser, 0|1, HASHREF)
{
    my ($parser, $start, $atts) = @_;
    my $webject = $atts->{'webject'};
    my $as = $atts->{'as'};
    
    if( $start ) {
        while(1) {
            last if $webject;
            last if $as;
            croak q(incomplete 'use' tag, use like this: <mp:use webject="SomeWebjectClass" as="Alias" />!);
            return;
        }
    }
    
    $parser->_aliases->{$as} = $webject;
    $parser->_append(MetaPage::_INCLUDES_, "use $webject;");
}

1;

#
# Copyright (C) 2010,2011 Vladislav Trifochkin
# MetaPage::HandlerFactory is a part of Met@Page software
# Met@Page is free software; you can redistribute it and/or
# modify it under the same terms as Perl itself.
#
package MetaPage::HandlerFactory::Plain;
use Carp;
use Text::Simplify;
use MetaPage::Parser;

use strict;
use warnings;

=head1
    
    MetaPage::HandlerFactory->handlersFor( $parser )
    
=cut

sub _PARENTS_   {'.parents'}

my %_handlers = (
    Start        => \&_on_start_elem,
    End          => \&_on_end_elem,
    Char         => \&_on_text,
);

my %_mp_handlers = (
    'use' => \&_on_mp_use,
    'var' => \&_on_mp_var,
    'if'  => \&_on_mp_if,
);


sub handlersFor {
    my ($class, $parser) = @_;
    $parser->setHandlers ( %_handlers );
    return $parser;
}


#sub _stringify_atts
#{
#    my @pairs = ();
#    while( my ($k,$v) = each(%{$_[0]}) ) {
#        push @pairs, $k. "=\"$v\"";
#    }
#    return join( ' ', @pairs);
#}


# 
sub _parse_expr
{
    local $_ = (@_ ?  $_[0] : $_ );
    s/([^\\]?)\$([\w_][\w\d_]+)/$1\$__vars->\{'$2'\}/sg;
    return $_;
}

sub _stringify_args
{
    my @args = ();
    foreach( @_ ) {
        $_ = _parse_expr;
        push @args, qq("$_");
    }
    return join( ',', @args);
}


sub _atts_to_webject_setters
{
    my $atts = $_[0];
    if( %$atts ) {
        my @setters = ();
        while( my ($k,$v) = each( %$atts ) ) {
            $v = _parse_expr($v);
            push @setters, "->$k(\"$v\")";
        }
        return @setters ? join('', @setters) : '';
    }
    return '';
}


sub _append_vname
{
    my ($parser, $class, $alias, $setters) = @_;
    
    my $vname = sprintf('$_%d_%s', ++$parser->{'webject_counter'}, $alias );

    $parser->_append(MetaPage::Parser::_TEXT_, 
        sprintf('my %s = %s->new()%s;', $vname, $class, $setters || ''));
    
    my $parents = $parser->{&_PARENTS_};
    my $parent = $parents->[scalar(@$parents)-1];
    $parser->_append(MetaPage::Parser::_TEXT_, sprintf('%s->add(%s);', $parent, $vname));
    push @{$parents}, $vname;
}

sub _on_start_elem # (Expat, Element [, Attr, Val [,...]])
{
    my ($parser, $elem, %atts) = @_;
    
    if( $parser->_is_root_elem($elem) ) {
        $parser->{'webject_counter'} = 0;
        $parser->{&_PARENTS_} = [];
        
        $parser->_aliases->{'Native'} = 'Webject::Native';
        $parser->_aliases->{'Text'} = 'Webject::Text';
        $parser->_append(MetaPage::Parser::_INCLUDES_,
            sprintf(q(use Webject::Media '%s';), $parser->media),
            'use Webject;',
            'use Webject::Native;',
            'use Webject::Text;'
        );
        $parser->_append(MetaPage::Parser::_TEXT_, 
            '{',
            'local $_;',
            'my $__vars = shift @ARGV || {};',
            '$_ = Webject->new();'
        );
        
        push @{$parser->{&_PARENTS_}}, '$_';

    } elsif ( $elem =~ /^mp:(.+)$/ ) {
        my $alias = $1;
        
        if( exists $_mp_handlers{$alias} ) {
            $_mp_handlers{$alias}->($parser, 1, \%atts);
        } else {
            my $class = $parser->_aliases->{$alias};
            
            $parser->xpcroak(sprintf( q(%s: unknown alias, use 'use' tag for define it!), $alias ))
                unless $class;
            
            my $setters = _atts_to_webject_setters(\%atts);
            #if( %atts ) {
            #    my @setters = ();
            #    while( my ($k,$v) = each(%atts) ) {
            #        push @setters, "->$k('$v')";
            #    }
            #    $setters = @setters ? join('', @setters) : '';
            #}
            _append_vname($parser, $class, $alias, $setters);
        }
    } else {
        my $alias = 'Native';
        my $class = 'Webject::Native';
        
        my $setters = "->tag('$elem')";
        if( %atts ) {
            $setters .= '->add_atts(' . _stringify_args(%atts) .')';
        } 
        _append_vname($parser, $class, $alias, $setters);
    }
}

sub _on_end_elem # (Expat, Element)
{
    my ($parser, $elem) = @_;
    if( $parser->_is_root_elem($elem) ) {
        $parser->_append(MetaPage::Parser::_TEXT_,
            'return $_->render;',
            '}'
        );
    } elsif ( $elem =~ /^mp:(.+)$/ ) {
        if ( exists $_mp_handlers{$1} ) {
            $_mp_handlers{$1}->($parser, 0);
        } else {
            pop @{$parser->{&_PARENTS_}};
        }
    } else {
        pop @{$parser->{&_PARENTS_}};
    }
}

sub _on_text # (Expat, String)
{
    my ($parser, $text, $not_quoted) = @_;
    simplify($text);
    if( $text ) { # text is not empty
        my $vname = sprintf('$_%d_Text', ++$parser->{'webject_counter'} );
        my $class = $parser->_aliases->{'Text'};
        
        $text = "'$text'" unless $not_quoted;
        
        $parser->_append(MetaPage::Parser::_TEXT_, 
            sprintf('my %s = %s->new()->value(%s);', $vname, $class, $text));
        my $parents = $parser->{&_PARENTS_};
        my $parent = $parents->[scalar(@$parents)-1];
        $parser->_append(MetaPage::Parser::_TEXT_, sprintf('%s->add(%s);', $parent, $vname));
    }
}

sub _on_mp_use # (parser, 0|1, HASHREF)
{
    my ($parser, $start, $atts) = @_;
    
    if( $start ) {
        my $webject = $atts->{'webject'};
        my $as = $atts->{'as'};
        while(1) {
            last if $webject;
            last if $as;
            $parser->xpcroak(q(incomplete 'use' tag, use like this: <mp:use webject="SomeWebjectClass" as="Alias" />!));
            return;
        }
        $parser->_aliases->{$as} = $webject;
        $parser->_append(MetaPage::Parser::_INCLUDES_, "use $webject;");
    }
}


sub _on_mp_var # (parser, 0|1, HASHREF)
{
    my ($parser, $start, $atts) = @_;
    
    if( $start ) {
        my $name = $atts->{'name'};
        my $val  = $atts->{'set'};
        unless( defined $val ) {
            _on_text($parser, "(\$__vars->{'$name'} || '')", 1);
        } else {
            $parser->_append(MetaPage::Parser::_TEXT_, "\$__vars->{'$name'} = \"$val\";");
        }
    }
}


sub _on_mp_if # (parser, 0|1, HASHREF)
{
    my ($parser, $start, $atts) = @_;
    
    if( $start ) {
        local $_ = $atts->{'test'};
        my $vars = $parser->metapage->vars;
        $parser->xpcroak("'test' attribute for 'if' statement expected and it must be not empty") unless $_;
        
        $_ = _parse_expr($_);
        
        $parser->_append(MetaPage::Parser::_TEXT_, "if( $_ ) {");
                         
    } else { # end
        $parser->_append( MetaPage::Parser::_TEXT_, '}' );
    }
}

1;

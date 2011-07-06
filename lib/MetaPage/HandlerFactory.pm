#
# Copyright (C) 2010 Vladislav Trifochkin
# MetaPage::HandlerFactory is a part of Met@Page software
# Met@Page is free software; you can redistribute it and/or
# modify it under the same terms as Perl itself.
#
package MetaPage::HandlerFactory;
use Module::Load;
use Module::Loaded;
use Params::Validate qw(:all);
use JQ::Logger;
use JQ::Utils::Text::Trim;
use MetaPage::Exception;
use strict;

use constant ROOT_ELEM => 'metapage';

=head
    MetaPage::HandlerFactory->handlersFor( $xml_parser )
=cut
sub handlersFor {
    my ($class, $xml_parser) = validate_pos( @_, 1,
        {
            type=>OBJECT,
            callbacks=>{
                'MetaPage::Parser object' => sub { ref $_[0] && ref $_[0] eq 'MetaPage::Parser' }
            }
        }
    );
    $xml_parser->setHandlers (
        Start        => \&_onStartElement,
        End          => \&_onEndElement,
        Char         => \&_onText,
        Proc         => \&_onProc,
        Comment      => \&_onComment,
        CdataStart   => \&_onCDataStart,
        CdataEnd     => \&_onCDataEnd,
        Default      => \&_onDefault,
        Unparsed     => \&_onUnparsed,
        Notation     => \&_onNotation,
        ExternEnt    => \&_onExternEnt,
        ExternEntFin => \&_onExternEntFin,
        Entity       => \&_onEntity,
        Element      => \&_onElement,
        Attlist      => \&_onAttlist,
        Doctype      => \&_onDoctype,
        DoctypeFin   => \&_onDoctypeFin,
        XMLDecl      => \&_onXmlDecl
    );
    $xml_parser->indent(0);
    $xml_parser->increment(4);

    my $elements = $xml_parser->elements;
}

sub _onStartElement # (Expat, Element [, Attr, Val [,...]])
{
    my $xml_parser = shift or die;
    my $elem = shift or die;
    my %atts = @_;

    my $elements = $xml_parser->elements;

    if( %{$elements} ) {
        MetaPage::Exception::ParseError->throw("unknown element: $elem" )
            unless exists $elements->{$elem};

        SWITCH: {
            $elem eq 'use' and do {
                SWITCH_0: {
                    MetaPage::Exception::XMLAttributeExpected->throw( "attribite 'as' expected for element '$elem'") unless exists $atts{'as'};
                    exists $atts{'webject'} and do {
                        $xml_parser->addCode( sprintf("use %s;", $atts{'webject'}) );
                        $elements->{$atts{'as'}} = $atts{'webject'};
                        last SWITCH_0;
                    };
                    exists $atts{'layout'} and do {
                        $elements->{$atts{'as'}} = $atts{'layout'};
                        $xml_parser->addCode( sprintf("use %s;", $atts{'layout'}) );
                        last SWITCH_0;
                    };
                    MetaPage::Exception::XMLUnexpectedAttribute->throw("unexpected attribute for '$elem'");
                }
                last SWITCH;
            };

            exists $elements->{$elem} and do {
                $xml_parser->addCode( sprintf('%s->new(', $elements->{$elem}) );
                foreach my $a ( keys %atts ) {
                    $xml_parser->addCode( sprintf(' %s=>"%s"', $a, $atts{$a} ) );
                }
                $xml_parser->addCode(");");
                last SWITCH;
            }

        }

        #my $pkgName = "MetaPage::Parser::$elem";
        #load $pkgName;
        #die ExParseError($xml_parser, "unable to load module: $pkgName, check if it exists" )
        #    unless is_loaded($pkgName);
        #$pkgName->parse( $xml_parser, $elem, %atts );

    } else {
        MetaPage::Exception::ParseError->throw(
            sprintf("may be it is not MetaPage resource file: root element '%s' expected", ROOT_ELEM) )
            unless $elem eq ROOT_ELEM;
        $elements->{'use'} = 1;
    }
}

sub _onEndElement # (Expat, Element)
{
    my ($xml_parser, $elem) = @_;
}

sub _onText # (Expat, String)
{
    my $xml_parser = shift or die;
    my $text  = shift or die;

    $text = sppacktrim($text);
    print ' 'x$xml_parser->indent, "$text\n" if $text;
}

sub _onProc # (Expat, Target, Data)
{
    my ($xml_parser, $target, $data) = @_;
    MetaPage::Exception::XMLUnexpectedEvent->throw("unexpected event 'Proc'")
        unless $xml_parser->ignoreXMLUnexpectedEvent;
}

sub _onComment # (Expat, Data)
{
    my $xml_parser = shift or die;
    my $text  = shift or die;
}

sub _onCDataStart # (Expat)
{
    my $xml_parser = shift or die;
    MetaPage::Exception::XMLUnexpectedEvent->throw("unexpected event 'CDataStart'")
        unless $xml_parser->ignoreXMLUnexpectedEvent;
}

sub _onCDataEnd # (Expat)
{
    my $xml_parser = shift or die;
    MetaPage::Exception::XMLUnexpectedEvent->throw("unexpected event 'CDataEnd'")
        unless $xml_parser->ignoreXMLUnexpectedEvent;
}

sub _onDefault # (Expat, String)
{
    #my $xml_parser = shift;
    #my $text  = shift;
}

sub _onUnparsed # (Expat, Entity, Base, Sysid, Pubid, Notation)
{
    my ($xml_parser, $entity, $base, $sysid, $pubid, $notation) = @_;
    MetaPage::Exception::XMLUnexpectedEvent->throw("unexpected event 'Unparsed'")
        unless $xml_parser->ignoreXMLUnexpectedEvent;
}

sub _onNotation # (Expat, Notation, Base, Sysid, Pubid)
{
    my ($xml_parser, $notation, $base, $sysid, $pubid) = @_;
    MetaPage::Exception::XMLUnexpectedEvent->throw("unexpected event 'Notation'")
        unless $xml_parser->ignoreXMLUnexpectedEvent;
}

sub _onExternEnt # (Parser, Base, Sysid, Pubid)
{
    my ($xml_parser, $base, $sysid, $pubid ) = @_;
    MetaPage::Exception::XMLUnexpectedEvent->throw("unexpected event 'ExternEnt'")
        unless $xml_parser->ignoreXMLUnexpectedEvent;
}

sub _onExternEntFin # (Expat)
{
    my $xml_parser = shift or die;
    MetaPage::Exception::XMLUnexpectedEvent->throw("unexpected event 'ExternEntFin'")
        unless $xml_parser->ignoreXMLUnexpectedEvent;
}

sub _onEntity # (Expat, Name, Val, Sysid, Pubid, Ndata, IsParam)
{
    my ($xml_parser, $name, $val, $sysid, $pubid, $ndata, $isparam) = @_;
    MetaPage::Exception::XMLUnexpectedEvent->throw("unexpected event 'Entity'")
        unless $xml_parser->ignoreXMLUnexpectedEvent;
}

sub _onElement # (Expat, Name, Model)
{
    my ($xml_parser, $name, $model) = @_;
    MetaPage::Exception::XMLUnexpectedEvent->throw("unexpected event 'Element'")
        unless $xml_parser->ignoreXMLUnexpectedEvent;
}

sub _onAttlist # (Expat, Elname, Attname, Type, Default, Fixed)
{
    my ($xml_parser, $elname, $attname, $type, $default, $fixed) = @_;
    MetaPage::Exception::XMLUnexpectedEvent->throw("unexpected event 'Attlist'")
        unless $xml_parser->ignoreXMLUnexpectedEvent;
}

sub _onDoctype # (Expat, Name, Sysid, Pubid, Internal)
{
    my ($xml_parser, $name, $sysid, $pubid, $internal) = @_;
    MetaPage::Exception::XMLUnexpectedEvent->throw("unexpected event 'Doctype'")
        unless $xml_parser->ignoreXMLUnexpectedEvent;
}

sub _onDoctypeFin # (Parser)
{
    my $xml_parser = shift or die;
    MetaPage::Exception::XMLUnexpectedEvent->throw("unexpected event 'DoctypeFin'")
        unless $xml_parser->ignoreXMLUnexpectedEvent;
}

sub _onXmlDecl # ( Expat, Version, Encoding, Standalone )
{
    my ($xml_parser, $version, $encoding, $standalone ) = @_;
    #die ExXMLUnexpectedEvent( sprintf('XmlDecl [version=%s; encoding=%s; standalone=%s]',
    #    $version // '<unknown>', $encoding // '<unknown>',
    #    defined($standalone) ? ($standalone ? 'yes' : 'no') : 'no') )
    #    unless $xml_parser->ignoreXMLUnexpectedEvent;
}

1;

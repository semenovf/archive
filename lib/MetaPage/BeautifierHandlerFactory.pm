#
# Copyright (C) 2010 Vladislav Trifochkin
# MetaPage::HandlerFactory is a part of Met@Page software
# Met@Page is free software; you can redistribute it and/or
# modify it under the same terms as Perl itself.
#
package MetaPage::BeautifierHandlerFactory;
use strict;


=head
    MetaPage::HandlerFactory->handlersFor( $xml_parser )
=cut
sub handlersFor {
    my $class = shift or die;
    my $xml_parser = shift or die;
    die ExBadType('xml_parser', 'MetaPage::Parser') unless (ref($xml_parser) eq 'MetaPage::Parser');

    $xml_parser->setHandlers (
        #Init         => \&_onInit,
        #Final        => \&_onFinal,
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
    $xml_parser->increment(2);
}

sub _onStartElement # (Expat, Element [, Attr, Val [,...]])
{
    my $xml_parser = shift or die;
    my $elem = shift or die;
    my @atts = @_;

    my @xml = ();
    push @xml, '<'.$elem;

    for(my $i = 0; $i <= $#atts; $i+=2) {
        push @xml, sprintf('%s="%s"', $atts[$i], $atts[$i+1]);
    }
    my $xml = join( ' ', @xml );
    $xml .= '>';
    print ' 'x$xml_parser->indent, $xml, "\n";
    $xml_parser->incrementIndent();
}

sub _onEndElement # (Expat, Element)
{
    my ($xml_parser, $elem) = @_;

    $xml_parser->decrementIndent();
    print ' 'x$xml_parser->indent, "</$elem>\n";
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
    print "=== _onProc() ===\n";
    print "(nothing-to-do)\n";
}

sub _onComment # (Expat, Data)
{
    my $xml_parser = shift or die;
    my $text  = shift or die;
    print "=== _onComment() ===\n";
    print "Comment: \"$text\"\n";
}

sub _onCDataStart # (Expat)
{
    print "=== _onCDataStart() ===\n";
    print "(nothing-to-do)\n";
}

sub _onCDataEnd # (Expat)
{
    print "=== _onCDataEnd() ===\n";
    print "(nothing-to-do)\n";
}

sub _onDefault # (Expat, String)
{
    #my $xml_parser = shift;
    #my $text  = shift;
}

sub _onUnparsed # (Expat, Entity, Base, Sysid, Pubid, Notation)
{
    print "=== _onUnparsed() ===\n";
    print "(nothing-to-do)\n";
}

sub _onNotation # (Expat, Notation, Base, Sysid, Pubid)
{
    print "=== _onNotation() ===\n";
    print "(nothing-to-do)\n";
}

sub _onExternEnt # (Parser, Base, Sysid, Pubid)
{
    my ($xml_parser, $base, $sysid, $pubid ) = @_;
    print "=== _onExternEnt() ===\n";
    print "base=[$base]; sysid=[$sysid]; pubid=[$pubid]\n";
}

sub _onExternEntFin # (Expat)
{
    print "=== _onExternEntFin() ===\n";
    print "(nothing-to-do)\n";
}

sub _onEntity # (Expat, Name, Val, Sysid, Pubid, Ndata, IsParam)
{
    print "=== _onEntity() ===\n";
    print "(nothing-to-do)\n";
}

sub _onElement # (Expat, Name, Model)
{
    print "=== _onElement() ===\n";
    print "(nothing-to-do)\n";
}

sub _onAttlist # (Expat, Elname, Attname, Type, Default, Fixed)
{
    print "=== _onAttlist() ===\n";
    print "(nothing-to-do)\n";
}

sub _onDoctype # (Expat, Name, Sysid, Pubid, Internal)
{
    print "=== _onDoctype() ===\n";
    print "(nothing-to-do)\n";
}

sub _onDoctypeFin # (Parser)
{
    print "=== _onDoctypeFin() ===\n";
    print "(nothing-to-do)\n";
}

sub _onXmlDecl # ( Expat, Version, Encoding, Standalone )
{
    my ($xml_parser, $version, $encoding, $standalone) = @_;
    my @xml = ();
    push @xml, '<?xml';
    push @xml, sprintf('version="%s"', $version);
    push @xml, sprintf('encoding="%s"', $encoding // '');
    push @xml, sprintf('standalone="%s"', defined($standalone) ? ($standalone ? 'yes' : 'no') : 'no' );
    push @xml, '?>';
    print join( ' ', @xml ), "\n";
}

1;

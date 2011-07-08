#!/usr/bin/perl -w

#
# Copyright (C) 2010,2011 Vladislav Trifochkin
# This file is a part of Met@Page software
# Met@Page is free software; you can redistribute it and/or
# modify it under the same terms as Perl itself.
#

use lib qw(../lib ../../Webject/lib ../../Text-Simplify/lib);
use MetaPage;
use MetaPage::HandlerFactory::Plain;
#use Perl::Tidy;
use strict;

my $mp = MetaPage->new;
MetaPage::HandlerFactory::Plain->handlersFor($mp);

local $/;
$mp->parse_text(<DATA>);
#Perl::Tidy::perltidy(
#    source=>$mp->to_string
#);
$mp->render;

__DATA__

<metapage>
    <mp:use webject="Webject::Text" as="Text" /> <!-- Text is a mandatory definition (Text) -->
    <mp:use webject="Webject::Window" as="Window" />
    <mp:use webject="Webject::HLine" as="HLine" />
    <mp:use webject="Webject::Table" as="Table" />
    <mp:use webject="Webject::Table::Section" as="Section" />
    <mp:use webject="Webject::Table::Row" as="Row" />
    <mp:use webject="Webject::Table::Cell" as="Cell" />
    
    <mp:Window title="first - MetaPage example">
    <div id="content">
        <mp:Table>
            <mp:Section>
                <mp:Row>
                    <mp:Cell>One</mp:Cell>
                    <mp:Cell>Two</mp:Cell>
                    <mp:Cell>Three</mp:Cell>
                </mp:Row>
            </mp:Section>
        </mp:Table>
        
        This is text
        
        <mp:HLine />
    </div>
    </mp:Window>
</metapage>
#!/usr/bin/perl -w

#
# Copyright (C) 2010,2011 Vladislav Trifochkin
# This file is a part of Met@Page software
# Met@Page is free software; you can redistribute it and/or
# modify it under the same terms as Perl itself.
#

use lib qw(../lib ../../Webject/lib);
use MetaPage;
use MetaPage::HandlerFactory;
use strict;

my $mp = MetaPage->new;
MetaPage::HandlerFactory->handlersFor($mp);

local $/;
$mp->parse_text(<DATA>);
$mp->render;

__DATA__

<metapage>
    <mp:use webject="Webject::Window" as="Window" />
    <mp:use webject="Webject::HLine" as="HLine" />
    <mp:use webject="Webject::Table" as="Table" />
    <mp:use webject="Webject::Section" as="Section" />
    <mp:use webject="Webject::Row" as="Row" />
    <mp:use webject="Webject::Cell" as="Cell" />
    
    <mp:Window title="first - MetaPage example">
        <mp:Table>
            <mp:Section>
                <mp:Row>
                    <mp:Cell>One</mp:Cell>
                    <mp:Cell>Two</mp:Cell>
                    <mp:Cell>Three</mp:Cell>
                </mp:Row>
            </mp:Section>
        </mp:Table>
        
        <mp:Hline />
    </mp:Window>
</metapage>
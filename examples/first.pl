#!/usr/bin/perl -w

#
# Copyright (C) 2010,2011 Vladislav Trifochkin
# This file is a part of Met@Page software
# Met@Page is free software; you can redistribute it and/or
# modify it under the same terms as Perl itself.
#
use lib qw(
    /home/u2007/perl
    /home/u2007/perl/share/perl/5.10
    /home/u2007/jobquest.ru/www/labmin/Labmin/lib
    /home/u2007/jobquest.ru/www/labmin/MetaPage/lib
    /home/u2007/jobquest.ru/www/labmin/Text-Simplify/lib
    /home/u2007/jobquest.ru/www/labmin/Webject/lib
    /home/u2007/jobquest.ru/www/labmin/Dancer-Template-MetaPage/lib
    /home/u2007/jobquest.ru/www/labmin/DBI-DDI/lib
    /home/u2007/jobquest.ru/www/labmin/Validator-Var/lib
);
use Webject::Media 'html';
use Data::Dumper;
use Webject;
use Webject::Native;
use Webject::Text;
use Webject::Window;
use Webject::HLine;
package header;
use Webject::Media 'html';
use Webject;
use Webject::Native;
use Webject::Text;
use Webject::HRef;
use strict;
use warnings;
no warnings qw( redefine );
sub _process {
local $_;
my $_vars = $ARGV[0] || {};
$_ = Webject->new();
my $_1_Native = Webject::Native->new()->tag('div')->set_attributes(qw(style id))->style('width: 100%; height: 1.5em; background-color: #333; color:#AAA; padding: 0;')->id('header');
$_->add($_1_Native);
my $_2_Native = Webject::Native->new()->tag('div')->set_attributes(qw(style))->style('position: relative; height:100%;');
$_1_Native->add($_2_Native);
my $_3_Native = Webject::Native->new()->tag('p')->set_attributes(qw(style))->style('position: absolute; right: 0; margin:0;');
$_2_Native->add($_3_Native);
my $_4_A = Webject::HRef->new()->href(qq($_vars->{'logout_uri'}))->style('color: inherit');
$_3_Native->add($_4_A);
my $_5_Text = Webject::Text->new()->value('Logout');
$_4_A->add($_5_Text);
return $_->render;
}



package main;
package footer;
use Webject::Media 'html';
use Webject;
use Webject::Native;
use Webject::Text;
use strict;
use warnings;
no warnings qw( redefine );
sub _process {
local $_;
my $_vars = $ARGV[0] || {};
$_ = Webject->new();
my $_1_Native = Webject::Native->new()->tag('div')->set_attributes(qw(style id))->style('width: 100%; height: 10em; background-color: #333; color:#AAA; padding: 0;')->id('footer');
$_->add($_1_Native);
my $_2_Native = Webject::Native->new()->tag('div')->set_attributes(qw(style))->style('position: relative; height:100%;');
$_1_Native->add($_2_Native);
my $_3_Native = Webject::Native->new()->tag('p')->set_attributes(qw(style))->style('position: absolute; left: 0; margin:0; height:100%');
$_2_Native->add($_3_Native);
my $_4_Text = Webject::Text->new()->value('Powered by MetaPage');
$_3_Native->add($_4_Text);
my $_5_Native = Webject::Native->new()->tag('p')->set_attributes(qw(style))->style('position: absolute; right: 0; margin:0;');
$_2_Native->add($_5_Native);
my $_6_Text = Webject::Text->new()->value('(C) Copyright Fedor Semenov, 2011');
$_5_Native->add($_6_Text);
return $_->render;
}



package main;
use strict;
use warnings;
no warnings qw( redefine );
sub _process {
local $_;
my $_vars = $ARGV[0] || {};
$_vars = {'win_clas'=>'win_clas', 'win_id'=>'win_id', 'title'=>'title'};
$_ = Webject->new()->class('win')->id('id');
my $_1_Window = Webject::Window->new()->id(qq($_vars->{'win_id'}));#->class(qq($_vars->{'win_class'}))->title(qq($_vars->{'title'}));
$_->add($_1_Window);
$_1_Window->add(header::_process());
my $_2_Text = Webject::Text->new()->value(($_vars->{'content'} || ''));
$_1_Window->add($_2_Text);
my $_3_Native = Webject::Native->new()->tag('div')->set_attributes(qw(style))->style('clear: both; height: 20px;');
$_1_Window->add($_3_Native);
my $_4_Native = Webject::Native->new()->tag('br');
$_1_Window->add($_4_Native);
$_1_Window->add(footer::_process());
return $_->render;
}
_process();



__END__

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

#$mp->media('html');
#print $mp->render_code, "\n----------------------\n\n";
$mp->render;
warn $@ if $@;

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
            <mp:Section type="body">
                <mp:Row>
                    <mp:Cell>One</mp:Cell>
                    <mp:Cell>Two</mp:Cell>
                    <mp:Cell>Three</mp:Cell>
                </mp:Row>
            </mp:Section>
            <mp:Section type="footer">
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
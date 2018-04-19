#!/usr/bin/perl -w

#
# Copyright (C) 2010,2011 Vladislav Trifochkin
# This file is a part of Met@Page software
# Met@Page is free software; you can redistribute it and/or
# modify it under the same terms as Perl itself.
#
use lib qw(
    ../lib
    ../../Webject/lib
    ../../Text-Simplify/lib
    ../../Labmin/lib
    ../../DBI-DDI/lib);



package login;
use Webject::Media 'html';
use Webject;
use Webject::Native;
use Webject::Text;
use Webject::Label;
use Webject::TextBox;
use Webject::Button;
use Webject::RichEdit;
use Labmin::Webject::ChangeLog;
use strict;
use warnings;
no warnings qw( redefine );
sub _process {
local $_;
my $_vars = $ARGV[0] || {};
$_ = Webject->new();


if( $_vars->{'error_msg'} ) {
my $_1_Native = Webject::Native->new();
$_1_Native->tag('p');
$_1_Native->set_attributes(qw(class));
$_1_Native->class('error');
$_->add($_1_Native);
my $_2_Native = Webject::Native->new();
$_2_Native->tag('strong');
$_1_Native->add($_2_Native);
my $_3_Text = Webject::Text->new();
$_3_Text->value('Error:');
$_2_Native->add($_3_Text);
my $_4_Text = Webject::Text->new();
$_4_Text->value(($_vars->{'error_msg'} || ''));
$_1_Native->add($_4_Text);
}


my $_5_Native = Webject::Native->new();
$_5_Native->tag('div');
my $t = $_5_Native->tag;
$_5_Native->set_attributes(qw(id));
$_5_Native->id('container');
my $id = $_5_Native->id;
$_->add($_5_Native);
my $_6_Native = Webject::Native->new();
$_6_Native->tag('form');
$_6_Native->set_attributes(qw(action method));
$_6_Native->action("/login");
$_6_Native->method('post');
$_5_Native->add($_6_Native);
my $_7_Label = Webject::Label->new();
$_6_Native->add($_7_Label);
my $_8_Text = Webject::Text->new();
$_8_Text->value('Username:');
$_7_Label->add($_8_Text);
my $_9_Native = Webject::Native->new();
$_9_Native->tag('br');
$_7_Label->add($_9_Native);
my $_10_TextBox = Webject::TextBox->new();
$_10_TextBox->name('username');
$_7_Label->add($_10_TextBox);
my $_11_Native = Webject::Native->new();
$_11_Native->tag('br');
$_6_Native->add($_11_Native);
my $_12_Label = Webject::Label->new();
$_6_Native->add($_12_Label);
my $_13_Text = Webject::Text->new();
$_13_Text->value('Password:');
$_12_Label->add($_13_Text);
my $_14_Native = Webject::Native->new();
$_14_Native->tag('br');
$_12_Label->add($_14_Native);
my $_15_TextBox = Webject::TextBox->new();
$_15_TextBox->name('password');
$_15_TextBox->type('password');
$_12_Label->add($_15_TextBox);
my $_16_Native = Webject::Native->new();
$_16_Native->tag('br');
$_6_Native->add($_16_Native);
my $_17_Button = Webject::Button->new();
$_17_Button->caption('Sign-in');
$_17_Button->type('submit');
$_6_Native->add($_17_Button);
my $_18_Native = Webject::Native->new();
$_18_Native->tag('div');
$_18_Native->set_attributes(qw(id));
$_18_Native->id('changelog');
$_->add($_18_Native);
my $_19_ChangeLog = Labmin::Webject::ChangeLog->new();
$_19_ChangeLog->limit('50');
$_18_Native->add($_19_ChangeLog);
return $_->render;
}
_process();

__END__

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
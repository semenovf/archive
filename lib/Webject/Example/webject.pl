#!/usr/bin/perl -w
use strict;
use lib '../../';
use Webject::Media 'html'; # use with media type only once in project
use Webject;
use Webject::Window;
use Webject::HLine;
use Webject::Label;
use Webject::TextBox;
use Webject::Table::Row;

my $win = Webject::Window->new;
my $label0 = Webject::Label->new->value('<BEFORE>')->where(Webject::Label::BEFORE);
my $label1 = Webject::Label->new->value('<AFTER>')->where(Webject::Label::AFTER);
my $hr0 = Webject::HLine->new->id("hr");
my $row0 = Webject::Table::Row->new;

$label0->add('Labeled Text: Label is BEFORE');
$label1->add('Labeled Text: Label is AFTER');

$row0->add('First Cell');
$row0->add('Second Cell');
$row0->add('Third Cell');

$win->add( $label0 );
$win->add( $hr0 );
$win->add( $label1 );
$win->add( 'This is test string' );
$win->add( Webject::TextBox->new->id('fisrt_name')->maxlength(32) );
$win->add( $row0 );

print $win->render;

0;

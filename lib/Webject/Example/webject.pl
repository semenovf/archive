#!/usr/bin/perl -w
use strict;
use lib '../../';
use Webject::Media 'html'; # use with media type only once in project
use Webject;
use Webject::Window;
use Webject::Table;
use Webject::HLine;
use Webject::Label;
use Webject::TextBox;
use Webject::Link;
use Webject::Button;
#use Webject::Table::Row;


my $win    = Webject::Window->new;
my $label0 = Webject::Label->new->value('<BEFORE>')->where(Webject::Label::BEFORE);
my $label1 = Webject::Label->new->value('<AFTER>')->where(Webject::Label::AFTER);
my $hr0    = Webject::HLine->new->id("hr");
my $table0 = Webject::Table->new->id("table0");
#my $row0 = Webject::Table::Row->new;

$label0->add('Labeled Text: Label is BEFORE');
$label1->add('Labeled Text: Label is AFTER');


my $row = $table0->header->new_row;
$row->new_cell->class('odd')->add('HeadCell0');
$row->new_cell->class('even')->add('HeadCell1');;
$row->new_cell->class('odd')->add('HeadCell2');;
$row->new_cell->class('even')->add('HeadCell3');;

$row = $table0->body->new_row;
$row->new_cell->class('odd')->add('BodyCell0');
$row->new_cell->class('even')->add('BodyCell1');;
$row->new_cell->class('odd')->add('BodyCell2');;
$row->new_cell->class('even')->add('BodyCell3');;

$row = $table0->footer->new_row;
$row->new_cell->class('odd')->add('FootCell0');
$row->new_cell->class('even')->add('FootCell1');;
$row->new_cell->class('odd')->add('FootCell2');;
$row->new_cell->class('even')->add('FootCell3');;

#$row0->add('First Cell');
#$row0->add('Second Cell');
#$row0->add('Third Cell');

$win->add( $label0 );
$win->add( $hr0 );
$win->add( $label1 );
$win->add( 'This is test string' );
$win->add( Webject::TextBox->new->id('fisrt_name')->maxlength(32) );
$win->add( $table0 );
$win->add( Webject::Link->new->href('http://perlinpower.com'));
$win->add( Webject::Button->new->type(Webject::Button::REGULAR)->caption('Regular button') );
$win->add( Webject::Button->new->type(Webject::Button::SUBMIT)->caption('Submit') );
$win->add( Webject::Button->new->type(Webject::Button::RESET)->caption('Reset') );

print $win->render;

0;

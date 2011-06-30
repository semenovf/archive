#!/usr/bin/perl -w
use strict;
use lib './lib';
use Webject::Window;
use Webject::HLine;

#print 'Start '. &{sub { my $str = 'middle'; $str; }} . ' End';

my $win = Webject::Window->new;
my $hr = Webject::HLine->new->id("id0");
#$win->add( $hr );
$win->add( $hr );
#$win->add( Webject::HLine->new->id("id0") );
#$win->add( Webject::HLine->new->id("id1")->class("class1") );
#$w->add( Webject::HLine->new->id("id2") );
#$w->add( Webject::HLine->new->id("id3") );
print $win->render;

#local $/;
#my $pos = tell DATA;
#my $wt = '\''.<DATA>.'\'';
#die unless $wt;
#$wt =~ s/<%(.*?)%>/\'\.\($1\)\.\'/sg;
#print eval $wt;


#my $text = '';
#$text.=<<EOT;
#Hello, World 0!
#EOT
#
#$text .= foo();
#$text .= bar();
#
#print $text, "\n";
#seek DATA, $pos, 0;
#
#$wt = <DATA>;
#die unless $wt;
#$wt =~ s/^\s*\n+\s*//;
#$wt =~ s/\s*\n+\s*$//;
#print $wt, "\n";

#my $eval_text = eval "'Evaled:'.foo().bar().\"\n\"";
#print $eval_text;

sub foo
{
    return q('foo subroutine');
}

sub bar
{
    return q('bar subroutine');
}

0;

__DATA__
<hr <%
foo()
%> />
<div <% bar() %> >
Hello, World!
</div>
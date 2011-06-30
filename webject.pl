#!/usr/bin/perl -w
use strict;
use lib './lib';
use Webject;
use Webject::HLine;

#print Webject->new->render, "\n";
#print Webject->new->render, "\n";
print Webject::HLine->new->render, "\n";

local $/;
my $pos = tell DATA;
#print "pos=$pos\n";
my $wt = '\''.<DATA>.'\'';
die unless $wt;
$wt =~ s/<%(.*?)%>/\'\.\($1\)\.\'/sg;
#print $wt, "\n";
print eval $wt;


#my $text = '';
#$text.=<<EOT;
#Hello, World 0!
#EOT
#
#$text.=<<EOT;
#Hello, World 1!
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
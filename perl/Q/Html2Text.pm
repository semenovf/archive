package JQ::Html2Text;

# Ave Wrigley Ave.Wrigley@itn.co.ukE

require 5.004;

#use Text::Format;
use JQ::Trim;

use strict;

our @ISA = qw(Exporter);
our @EXPORT = qw(html2text);

my $format_default = {
    'head'   => { ignore => 1 },
    'script' => { ignore => 1 },
    'br'    => "\n",        # string
};



sub html2text
{
    my $node = shift or die;
    my $format = shift || $format_default;   # configuration hash ref
    my $text = '';

    foreach my $c ($node->content_list) {

        unless( ref $c ) {      # text node
            $text .= $c;
            next;
        }

        'script' eq $c->tag and do {
            next if $format->{'head'}->{'ignore'};

            # not supported yet
            next;
        };

        'head' eq $c->tag and do {
            next if $format->{'head'}->{'ignore'};

            # not supported yet
            next;
        };

        'br' eq $c->tag and do {
            $text .= $format->{'br'} || '';
            next;
        };

        ('p' eq $c->tag or 'div' eq $c->tag) and do {
            $text .= "\n";
            $text .= html2text($c);
            next;
        };

        'tr' eq $c->tag and do {
            $text .= "\n";
            $text .= html2text($c);
            next;
        };

        ('th' eq $c->tag or 'td' eq $c->tag) and do {
            $text .= html2text($c);
            $text .= "\t";
            next;
        };

        $text .= html2text($c);
    }

    return trim($text) || '';
}


1;

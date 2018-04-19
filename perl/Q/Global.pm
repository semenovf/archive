package JQ::Global;
use Encode;
use strict;

require Exporter;
our @ISA = qw(Exporter);
our @EXPORT = qw( _T default_encoding _tr );

my $_encoding = undef;

sub default_encoding
{
    my $enc = shift or return $_encoding;
    die "Unknown or invalid global encoding" unless Encode::find_encoding($enc);
    $_encoding = $enc;
}

sub _T
{
    my @strlist = ();
    my $enc = default_encoding();
    if( $enc ) {
        foreach my $str (@_) {
            push @strlist, Encode::decode($enc, $str);
        }
    } else {
        foreach my $str (@_) {
            push @strlist, $str;
        }
    }
    return join('', @strlist);
}

# subroutine for dynamic translation
sub _tr($)
{
    return $_[0];
}

1;

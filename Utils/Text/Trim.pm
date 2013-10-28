package JQ::Utils::Text::Trim;
use strict;

require Exporter;
our @ISA = qw(Exporter);
our @EXPORT = qw( ltrim rtrim trim sppack sppacktrim );

=item ltrim( $string )
=item rtrim( $string )
=item trim( $string )
=item sppack ($string )
=item sppacktrim( $string )

Trims leftmost, rightmost and both spaces
Packs spaces

=cut

# \xA0 == chr(160) == &nbsp

sub ltrim {
    return unless $_[0];
    my $arg = shift;
    my $ptr;

    if( ref($arg ) ){
        $ptr = $arg ;
    } else {
        $ptr = \$arg;
    }

    $$ptr =~ s/^[\s\xA0]+//s;
    return $$ptr;
}

sub rtrim {
    return unless $_[0];
    my $arg = shift;
    my $ptr;

    if( ref($arg ) ){
        $ptr = $arg ;
    } else {
        $ptr = \$arg;
    }

    $$ptr =~ s/[\s\xA0]+$//s;
    return $$ptr;
}

sub trim {
    return unless $_[0];
    my $arg = shift;
    my $ptr;

    if( ref($arg ) ){
        $ptr = $arg ;
    } else {
        $ptr = \$arg;
    }
    
    $$ptr =~ s/^[\s\xA0]+//s;
    $$ptr =~ s/[\s\xA0]+$//s;
    return $$ptr;
}

sub sppack {
    return unless $_[0];
    my $arg = shift;
    my $ptr;

    if( ref($arg ) ){
        $ptr = $arg ;
    } else {
        $ptr = \$arg;
    }
    
    $$ptr =~ s/[\s\xA0]+/ /g;
    return $$ptr;
}

sub sppacktrim {
    return unless $_[0];
    my $arg = shift;

    my $ptr;
    if( ref($arg ) ){
        $ptr = $arg ;
    } else {
        $ptr = \$arg;
    }
    
    $$ptr =~ s/^[\s\xA0]+//s;
    $$ptr =~ s/[\s\xA0]+$//s;
    $$ptr =~ s/[\s\xA0]+/ /g;
    return $$ptr;
}

1;

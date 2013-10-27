package cwt::Trim;
use strict;

=item cwt::Trim::ltrim( $string )
=item cwt::Trim::rtrim( $string )
=item cwt::Trim::trim( $string )
=item cwt::Trim::sppack ($string )
=item cwt::Trim::sppacktrim( $string )

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

package Webject::Media::html;
use HTML::Entities;
use strict;

sub media { 'html' }

sub encode
{
    shift; # $self
    return encode_entities(shift || '', '<>&');
}


sub decode
{
    shift; # $self
    my $decoded = decode_entities(shift || '');
    return $decoded;
}


#
# stringify_atts( HASHREF )
# stringify_atts( ARRAYREF )
# stringify_atts( ARRAYREF, OBJECT )
#
sub stringify_atts
{
    my ($atts, $obj) = @_;
    my @atts_arr = ();

    if( ref $atts && ref $atts eq 'HASH' ) {
	while( my ($k,$v) = each(%$atts) ) {
	    push @atts_arr, "$k=\"$v\"";
	}
    } elsif( ref $atts && ref $atts eq 'ARRAY' ) {
	if( $obj ) {
	    local $_;
	    foreach( @$atts ) {
		push @atts_arr, "$_=\"" . $obj->$_() . '"' if $obj->can($_) && $obj->$_();
	    }
	} else {
	    for( my $i = 0; $i < @$atts; $i+=2 ) {
		push @atts_arr, $atts->[$i] . '="' . $atts->[$i] . '"';
	    }
	}
    }
    
    return @atts_arr ? join(' ', @atts_arr) : '';
}


#sub stringify_att
#{
#    my ($self, $webject, $att_name, $method) = @_;
#    $method = $att_name unless defined $method;
#    if( $webject->can( $method ) ) {
#	return sprintf(q(%s="%s"), $att_name, $self->encode($webject->$method()))
#	    if defined $webject->$method();
#    }
#    return '';
#}

1;

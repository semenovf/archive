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


sub stringify_atts
{
    my $self = shift or die;
    my %atts = @_;
    my $atts_str = '';

    while( my ($k,$v) = each(%atts) ) {
	$atts_str .= ' '. $k .'="'. $v .'"';
    }
    
    return $atts_str;
}


sub stringify_att
{
    my ($self, $webject, $att_name, $method) = @_;
    $method = $att_name unless defined $method;
    if( $webject->can( $method ) ) {
	return sprintf(q(%s="%s"), $att_name, $self->encode($webject->$method()))
	    if defined $webject->$method();
    }
    return '';
}

1;

package Webject::Media::json;
use strict;

sub media { 'json' }

sub encode
{
    return $_[0];
}


sub decode
{
    return $_[0];
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
    return '';
}

1;

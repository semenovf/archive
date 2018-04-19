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

# @TODO
sub stringify_atts
{
    my $self = shift or die;
    my $atts_str = '';
    return $atts_str;
}


#sub stringify_att
#{
#    my ($self, $webject, $att_name, $method) = @_;
#    return '';
#}

1;

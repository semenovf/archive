package Webject::Link;
use base Webject;
use strict;


#
# This webject is used as direct child for Window webject.
# In other cases instances of this class don't have any effect.
#
# my $win = Webject->Window->new;
# $win->add(Webject::Link->new()->type(ICO)->href($ref_ico));
# $win->add(Webject::Link->new()->type(CSS)->href($ref_css));
# $win->add(Webject::Link->new()->type(JAVASCRIPT)->href($ref_java_script));
#

sub ICO        {'ico'}
sub CSS        {'css'}
sub JAVASCRIPT {'javascript'}

__PACKAGE__->mk_accessors(qw(type href));

sub render
{
    my $self = shift;
    local $_ = $self->type; 
    $_ eq ICO and do { return sprintf('<link rel="icon" type="image/ico" href="%s" />', $self->href); };
    $_ eq CSS and do { return sprintf('<link rel="stylesheet" type="text/css" href="%s" />', $self->href); };
    $_ eq JAVASCRIPT and do { return sprintf('<script type="text/javascript" src="%s" />', $self->href); };
    return '';
}

1;


__DATA__

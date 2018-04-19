package Webject::Media;
use strict;
use vars qw(@ISA);

my $_media;

sub import {
    my $proto = shift;
    my $media = shift || 'html';
    $_media = $media unless defined $_media;
    require "Webject/Media/$_media.pm";
    @ISA = ("Webject::Media::$_media");
}

1;

__END__


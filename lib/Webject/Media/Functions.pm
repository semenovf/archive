package Webject::Media::Functions;
use Webject::Media;
use strict;
use vars qw(@ISA @EXPORT);

require Exporter;

@ISA = qw(Exporter);

@EXPORT = qw(
    media
    stringify_atts
    stringify_att
    encode
    decode
);

#my @unprefixed = map { $_ =~ s/^wm_(.+)/$1/ } @EXPORT;

#foreach (@unprefixed) {
foreach (@EXPORT) {
    my $sub = Webject::Media->can($_);
    no strict 'refs';
    *{$_} = sub {&$sub('Webject::Media', @_)};
}


1;

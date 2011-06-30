package Webject::Accessor;
use base Class::Accessor;

sub set {
    my $self = shift;
    $self->SUPER::set(@_);
    return $self;
}

1;
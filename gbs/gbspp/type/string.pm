package gbspp::type::string;
use base gbspp::type::scalar;
use strict;
use warnings;

sub new
{
    my $class = required(\@_);
    return $class->SUPER::new(@_);
}

#
# $string->assign($scalar)
# $string->assign($refscalar)
#
sub _assign
{
    my $self = required(\@_);
    my $value = required(\@_);

    return undef unless defined($value);

    if (ref($value) ne '') {
        if (ref($value) eq __PACKAGE__) {
            return qq{$value->{value}};
        } elsif (ref($value) eq 'SCALAR') {
            return qq{$$value};
        }
    } else {
        return qq{$value};
    }
    
    return undef;
};

sub _equals
{
    my $self  = required(\@_);
    my $value = required(\@_);
    
    return $self->value eq __PACKAGE__->new($value)->value;
}

1;
package gbspp::type::numeric;
use base gbspp::type::scalar;
use gbspp::sub qw(required);
use strict;
use warnings;

sub new
{
    my $class = required(\@_);
    return $class->SUPER::new(@_);
}

#
# $string->assign($number)
# $string->assign($refnumber)
#
sub _assign
{
    my $self = required(\@_);
    my $value = required(\@_);
    
    return undef unless defined($value);

    if (ref($value) ne '') {
        if (ref($value) eq __PACKAGE__) {
            return $value->{value};
        }
    } elsif (gbspp::type::scalar::_isnum($value)) {
        return $value;
    }

    return undef;
};

sub _equals
{
    my $self  = required(\@_);
    my $value = required(\@_);
    
    return $self->value == __PACKAGE__->new($value)->value;
}

1;
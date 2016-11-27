package gbspp::type::numeric;
use base gbspp::type::scalar;
use strict;
use warnings;

sub new
{
    my ($class, @args) = @_;
    return $class->SUPER::new(@args);
}

#
# $string->assign($number)
# $string->assign($refnumber)
#
sub _assign
{
    my ($self, $value) = @_;
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

1;
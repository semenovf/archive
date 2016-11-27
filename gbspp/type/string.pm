package gbspp::type::string;
use base gbspp::type::scalar;
use strict;
use warnings;

sub new
{
    my ($class, @args) = @_;
    return $class->SUPER::new(@args);
}

#
# $string->assign($scalar)
# $string->assign($refscalar)
#
sub _assign
{
    my ($self, $value) = @_;
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

1;
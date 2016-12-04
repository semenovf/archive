package gbspp::type::bool;
use base gbspp::type::scalar;
use strict;
use warnings;

sub new
{
    my ($class, @args) = @_;
    return $class->SUPER::new(@args);
}

#
# $bool->assing(0)
# $bool->assign(<non-zero-number>)
# $bool->assign('false')
# $bool->assign('true')
# $bool->assign('FALSE')
# $bool->assign('TRUE')
# $bool->assign($otherbool)
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
        if ($value == 0) {
            return 0;
        } else {
            return 1;
        }
    } else {
        if ($value eq 'false' or $value eq 'FALSE') {
            return 0;
        } elsif ($value eq 'true' or $value eq 'TRUE') {
            return 1;
        }
    }
    
    return undef;
};

sub _equals
{
    my ($self, $a) = @_;

    return $a->can('equals')
        ? $a->equals($self->value)
        : $self->value == _self->_assign($a);
}

1;

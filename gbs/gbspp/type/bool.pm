package gbspp::type::bool;
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
    my $self  = required(\@_);
    my $value = required(\@_);
    
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
    my $self  = required(\@_);
    my $value = required(\@_);
    
    return $self->value == __PACKAGE__->new($value)->value;
}

1;

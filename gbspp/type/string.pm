package gbspp::type::string;
use gbspp::type::scalar;
use strict;
use warnings;

sub new
{
    my ($class, $value) = @_;
    die unless defined($value);
    my $self = bless {}, $class;
    $self->assign($value);
    return $self;
}

#
# $bool->set($scalar)
# $bool->set($refscalar)
#
sub _assign
{
    my $value = shift;
    return undef unless defined($value);

    if (ref($value) ne '') {
        if (ref($value) eq __PACKAGE__) {
            return $value->{value};
        } elsif (ref($value) eq 'SCALAR') {
            return $$value;
        }
    } else {
        return $value;
    }
    
    return undef;
};

#
# $self->can_assign($value)
# $self->can_assign                  # Alwais Ok, can be assigned to itself.
# __PACKAGE__::can_assign($value)
#
sub can_assign
{
    my ($self, $value) = @_;
    
    return (defined($value))
        ? defined(_assign($value))
        : defined(_assign($self));
}

#
# $bool->set(0)
# $bool->set(<non-zero-number>)
# $bool->set('false')
# $bool->set('true')
# $bool->set('FALSE')
# $bool->set('TRUE')
# $bool->set($otherbool)
#
sub assign
{
    my ($self, $value) = @_;
    my $result = _assign($value);
    die unless defined($result);
    $self->{value} = $result;
    return $self;
}

sub value
{
    my ($self) = @_;
    return $self->{value};
}

1;
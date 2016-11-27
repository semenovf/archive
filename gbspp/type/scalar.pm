package gbspp::type::scalar;
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

sub _isnum
{
    return 0 if $_[0] eq '';
    $_[0] ^ $_[0] ? 0 : 1
};

#
# $self->can_assign($value)
# __PACKAGE__->can_assign($value)
#
sub can_assign
{
    my ($self, $value) = @_;
    
    return (defined($value))
        ? defined($self->_assign($value))
        : defined($self->_assign($value));
}

sub assign
{
    my ($self, $value) = @_;
    my $result = $self->_assign($value);
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

package gbspp::type::enum;
use base gbspp::type::scalar;
use gbspp::sub qw(required);
use Carp;
use strict;
use warnings;

#
# my $v = gbspp::type::enum->new('string' | 'bool' | 'numeric', @variants)
#
sub new
{
    my $class     = required(\@_);
    my $itemclass = required(\@_);
    my $variants  = required(\@_);

    croak("Expected array for variants") unless (ref($variants) eq 'ARRAY');

    unless ($itemclass =~ /::/) {
        $itemclass = 'gbspp::type::' . $itemclass;
    }

    eval "require $itemclass";

    my $variants1 = [];
    
    foreach my $v (@$variants) {
        push (@{$variants1}, $itemclass->new($v));
    }
    
    return $class->SUPER::new($variants1->[0], {'itemclass' => $itemclass, 'variants' => $variants1});
}

#
# $enum->assign(<valid-variant-value>)
# $enum->assign($otherenum)
#
sub _assign
{
    my $self = required(\@_);
    my $value = required(\@_);

    return undef unless defined($value);

    if (ref($value) eq __PACKAGE__) {
        return qq{$value->{value}};
    } else {
        croak("instantiated object expected") unless (ref($self) eq __PACKAGE__);
        
        my $itemclass = $self->{itemclass};
        my $variants  = $self->{variants};

        if ($itemclass->can_assign($value)) {
            foreach my $v (@$variants) {
                if ($itemclass->new($self->value)->equals($v)) {
                    return $value;
                }
            }
        }
    }
    
    return undef;
};

sub _equals
{
    my $self = required(\@_);
    my $value = required(\@_);

    my $itemclass = $self->{itemclass};
    
    return $itemclass->new($self->value)->equals($value);
}

1;
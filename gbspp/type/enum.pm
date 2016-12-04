package gbspp::type::enum;
use base gbspp::type::scalar;
use strict;
use warnings;

#
# my $v = gbspp::type::enum->new('string' | 'bool' | 'numeric', @variants)
#
sub new
{
    my ($class, $itemclass, $variants) = @_;
    die unless $class;
    die "Item class expected" unless $itemclass;
    die "No variants specified" unless defined($variants) && ref($variants) eq 'ARRAY' && @$variants > 0;
    
    unless ($itemclass =~ /::/) {
        $itemclass = 'gbspp::type::' . $itemclass;
    }

    eval "require $itemclass";
    
    foreach my $v (@$variants) {
        die "Inappropriate variant value: $v" unless $itemclass->can_assign($v);
    }
    
    my $self = $class->SUPER::new($variants->[0], {'itemclass' => $itemclass, 'variants' => $variants});
    
    return $self;
}

#
# $enum->assign(<valid-variant-value>)
# $enum->assign($otherenum)
#
sub _assign
{
    my ($self, $value) = @_;
    return undef unless defined($value);

    if (ref($value) eq __PACKAGE__) {
        return qq{$value->{value}};
    } else {
        die "Instantiated object expected" unless (ref($self) eq __PACKAGE__);
        
        my $itemclass = $self->{itemclass};
        my $variants = $self->{variants};

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
    my ($self, $a) = @_;
    my $itemclass = $self->{itemclass};
    
    return $itemclass->new($self->value)->equals($a);
}

1;
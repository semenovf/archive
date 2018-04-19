package gbspp::type::array;
use gbspp::sub qw(required);
use strict;
use warnings;

#
# my $array = gbspp::type::array->new(TYPE);
#   where
# TYPE - any type (short or full name) from gbspp::type::* scope
#
sub new
{
    my $class     = required(\@_);
    my $itemclass = required(\@_);
    
    unless ($itemclass =~ /::/) {
        $itemclass = 'gbspp::type::' . $itemclass;
    }

    eval "require $itemclass";
    
    return bless {
          'itemclass' => $itemclass
        , 'data'      => []
    }, $class;
}

sub size
{
    my $self  = required(\@_);
    return scalar(@{$self->{'data'}});
}

#
# my $value = $array->at($index)
#
sub at
{
    my $self  = required(\@_);
    my $index = required(\@_);
    
    croak("index is out of bounds") unless ($index >= 0 && $index < @{$self->{'data'}});
    
    return $self->{'data'}->[$index];
}

#
# $array->push_back($value)
#
sub push_back
{
    my $self  = required(\@_);
    my $value = required(\@_);
    my $itemclass = $self->{'itemclass'};
 
    push(@{$self->{'data'}}, $itemclass->new($value));
}

#
# $array->pop_back()
#
sub pop_back
{
    my $self  = required(\@_);
    pop(@{$self->{'data'}});
}

#
# my $var = $array->front()
#
sub front
{
    my $self  = required(\@_);
    croak("empty array") unless (scalar(@{$self->{'data'}}) > 0);
    return $self->{'data'}->[0];
}

#
# my $value = $array->back()
#
sub back
{
    my $self  = required(\@_);
    croak("empty array") unless (scalar(@{$self->{'data'}} > 0));
    return $self->{'data'}->[scalar(@{$self->{'data'}}) - 1];
}

1;

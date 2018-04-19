package JQ::Errorable;
use strict;
use warnings;

sub makeErrorable
{
    my $self = shift or die;
    $self->{-errors} = [];
    return $self;
}

#   if( $obj->isError ) { ... }
sub isError
{
    my $self = shift or die;
    return scalar(@{$self->{-errors}}) > 0 ? 1 : 0;
}

sub clearErrors
{
    my $self = shift or die;
    $self->{-errors} = [];
}

# 2. $obj->setError($errorMessage)
sub setError
{
    my $self = shift or die;

    if( scalar(@_) == 1) {
        push @{$self->{-errors}}, $_[0];
    } else {
        my $fmt = shift;
        push @{$self->{-errors}}, sprintf($fmt, @_);
    }
    1;
}

sub errors
{
    my $self = shift or die;
    return $self->{-errors};
}

sub lastError
{
    my $self = shift or die;
    my $sz = scalar( @{$self->{-errors}} );

    return '' if $sz == 0;
    return ${$self->{-errors}}[$sz-1];
    1;
}

1;

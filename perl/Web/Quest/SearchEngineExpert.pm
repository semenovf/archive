package JQ::Web::Quest::SearchEngineExpert;
use base 'JQ::Web::Quest::Expert';
use strict;
use warnings;

__PACKAGE__->mk_accessors(qw(
    _counter
));

sub new
{
    my $class = shift or die;
    my $self = bless $class->SUPER::new(@_), $class;
    $self->_resetCounter;
    return $self;
}

sub callback
{
    my $self = shift or die;
    $self->_incCounter;
    return $self->SUPER::callback(@_);
}

sub begin
{
    my $self = shift;
    $self->_resetCounter;
    return $self->SUPER::begin(@_);
}


sub _resetCounter
{
    my $self = shift or die;
    $self->_counter(0);
}

sub _incCounter
{
    my $self = shift or die;
    $self->_counter( $self->_counter+1 );
}

sub getCount
{
    my $self = shift or die;
    return $self->_counter;
}


1;

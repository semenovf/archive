package JQ::ClassLoader;

use Module::Load;
use strict;

sub new
{
    my $root_class = shift;
    my $sub_class = shift;
    my %args = @_;
    my $real_class = $root_class."::$sub_class";
    load $real_class;
    
    my $id = delete $args{-id} || '';
    my $self = $real_class->new(%args);
    $self->{'id'} = $id;
    
    return $self;
}

1;

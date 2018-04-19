package Webject::Iterator;
use base 'Webject';

#__PACKAGE__->mk_accessors(qw(rows));

sub ctor
{
    my $self = shift;
    $self->SUPER::ctor();
    $self->set_attributes( qw(name) );
    $self->id("webject_reach_edit");
    $self->name($self->id);
}

sub render
{
    
}

1;

__DATA__

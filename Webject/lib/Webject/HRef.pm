package Webject::HRef;
use base Webject;
use strict;

sub ctor
{
    my $self = shift;
    $self->SUPER::ctor();
    $self->set_attributes( qw(href) );
    $self->href('#');
}


1;


__DATA__

<!--html{
<a <%=$self->render_atts %> >
    <%=$self->render_children %>
</a>
}html-->

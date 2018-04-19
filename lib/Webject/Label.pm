package Webject::Label;
use base 'Webject';

sub ctor
{
    my $self = shift;
    $self->SUPER::ctor();
    $self->set_attributes( qw(for) );
}


1;

__DATA__

<!--html{
<label <%=$self->render_atts%>>
    <%=$self->render_children %>
</label>
}html-->



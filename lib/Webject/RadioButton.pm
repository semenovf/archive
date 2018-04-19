package Webject::RadioButton;
use base 'Webject';
use strict;

__PACKAGE__->mk_accessors(qw(checked));

sub ctor
{
    my $self = shift;
    $self->SUPER::ctor();
    $self->set_attributes( qw(value) );
}


__DATA__

<!--html{
<input type="radio" <%=$self->render_atts%>
    <% if( $self->checked ) { %> checked="1" <% } %>
/>
}html-->

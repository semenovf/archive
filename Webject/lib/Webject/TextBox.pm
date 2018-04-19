package Webject::TextBox;
use base 'Webject';

#
# Types
#
sub REGULAR      {'text'}
sub AUTOCOMPLETE {1}
sub PASSWORD     {'password'}

__PACKAGE__->mk_accessors(qw(rows));

sub ctor
{
    my $self = shift;
    $self->SUPER::ctor();
    $self->set_attributes( qw(name type value maxlength) );
    $self->type(REGULAR);
    $self->rows(1);
}

1;

__DATA__

<!--html{
<% if( $self->rows > 1 ) { %>
    <textarea <%=$self->render_atts%> rows=<%=$self->rows%>>
        <%=$self->render_children%>
    </textarea>
<% } else { %>
    <input <%=$self->render_atts%> />
<% } %>
}html-->

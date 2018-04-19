package Webject::Native;
use Webject::Media::Functions;
use base 'Webject';

__PACKAGE__->mk_accessors(qw(tag));

# Empty ctor is for ignoring default (Webject) initializations,
# such as the setting attributes.
sub ctor {}

1;

__DATA__

<!--html{
<% if( exists { 'br'=>1, 'hr'=>1 }->{$self->tag} ) { %>
    <<%=$self->tag%> <%=$self->render_atts %> />
<% } else { %>
    <<%=$self->tag%> <%=$self->render_atts %>>
        <%=$self->render_children %>
    </<%=$self->tag%>>
<% } %>
}html-->


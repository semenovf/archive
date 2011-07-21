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
<<%=$self->tag%> <%=$self->render_atts %>
    <% if( $self->children_count ) { %>
        >
        <%=$self->render_children %>
        </<%=$self->tag%>>
    <% } else { %>
    />
    <% } %>
}html-->


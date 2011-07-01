package Webject::HLine;
use base 'Webject';
1;

__DATA__

<!--html{
<!--This is a comment -->
<hr <%=$self->render_atts %> />
}html-->


<!--json{
{
    "hr": {
        <%=$self->render_atts %>
    }
}
}json-->
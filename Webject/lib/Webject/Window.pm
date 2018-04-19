package Webject::Window;
use base 'Webject';

use 5.006;
use strict;
use warnings;

__PACKAGE__->mk_accessors(qw(title charset));

sub ctor
{
    my $self = shift;
    $self->SUPER::ctor();
    #$self->set_attributes( qw() );
    $self->title('<untitled>');
    $self->charset('utf-8');
}


1;

__DATA__

<!-- HTML --------------------------------------------------------------------->

<!--html{
    <title><%=$self->title %></title>
    <meta http-equiv="Content-Type" content="text/html; charset=<%=$self->charset%>" />
</head>
<body <%=$self->render_atts%>>
    <%=$self->render_children %>
</body>
}html-->

<!-- JSON --------------------------------------------------------------------->

<!--json{
{ "window":
    {
        "title": "<%=$self->title%>",
        <%=$self->render_children %>
    }
}
}json-->

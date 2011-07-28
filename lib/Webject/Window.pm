package Webject::Window;
use base 'Webject';

use 5.006;
use strict;
use warnings;

#my $XHTML_1_0_Strict       = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">';
#my $XHTML_1_0_Transitional = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">';
#my $XHTML_1_0_Frameset     = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">';
#my $XHTML_1_0_Mobile       = '<!DOCTYPE html PUBLIC "-//WAPFORUM//DTD XHTML Mobile 1.0//EN" "http://www.wapforum.org/DTD/xhtml-mobile10.dtd">';
#my $XHTML_1_1              = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">';

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
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
<head>
    <title><%=$self->title %></title>
    <meta http-equiv="Content-Type" content="text/html; charset=<%=$self->charset%>" />
<%= join('', $self->fetch_clipboard('HEAD')) %>
</head>
<body <%=$self->render_atts%>>

<%=$self->render_children %>

</body>
</html>

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

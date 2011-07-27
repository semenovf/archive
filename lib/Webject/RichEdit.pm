package Webject::RichEdit;
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


        #\$(document).ready(
        #    function() {
        #        \$('#${id}').redactor();
        #    }
        #);


1;

__DATA__

<!--html{
<%
    my $id = $self->id;
    $self->push_clipboard('SCRIPT', q(<script type="text/javascript" src="/javascripts/redactor/redactor.js"></script>));
    $self->push_clipboard('STYLE', q(<link rel="stylesheet" href="/javascripts/redactor/css/redactor.css" type="text/css" />));
    $self->push_clipboard('SCRIPT', qq(
        <script type="text/javascript">
            \$(function()
            {
            \$('#redactor').redactor({ focus: true });
            });
    </script>));
%>


<textarea id="redactor" name="redactor" style="height:300px; width:99%;" <%=$self->render_atts%>>
    <%=$self->render_children%>
</textarea>
}html-->

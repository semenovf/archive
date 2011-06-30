package Webject::HLine;
use base 'Webject';
1;

__DATA__

<!--html{

<hr <%
    $self->render_attrs();
%> />

}html-->


<!--json{
    
}json-->
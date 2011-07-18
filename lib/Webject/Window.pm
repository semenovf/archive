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

my @rw_accessors = qw(
    title
    charset
    stylesheet
);

__PACKAGE__->mk_accessors(@rw_accessors);

sub new {
    my $class = shift;
    my $self = bless $class->SUPER::new(@_), $class;
    $self->title('<untitled>');
    $self->charset('utf-8');
    return $self;
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
    <link rel="icon" href="/images/favicon.ico" type="image/ico" />
    
<%
    my @ss = split('\s+', $self->stylesheet || '');
    foreach ( @ss ) {
%>
    <link rel="stylesheet" href="<%=$_%>" type="text/css" />
<%  }%>

    <!--[if lte IE 7]>
    <!-- <link rel="stylesheet" href="/css/ie.css" type="text/css" /> -->
    <![endif]--> 
    <!--[if lte IE 6]>
    <!-- <link rel="stylesheet" href="/css/ie6.css" type="text/css" /> -->
    <![endif]--> 
    
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

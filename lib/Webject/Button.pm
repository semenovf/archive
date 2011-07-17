package Webject::Button;
use base 'Webject';
use strict;

sub REGULAR { 'button' }
sub SUBMIT  { 'submit' }
sub RESET   { 'reset' }

my @rw_accessors = qw(
    value
    type
    accesskey
);

__PACKAGE__->mk_accessors(@rw_accessors);


sub new
{
    my $class = shift;
    my $self = $class->SUPER::new(@_);
    $self->caption('ok');
    $self->type(SUBMIT);
    return bless $self, $class;
}

# synonim to 'value' method
sub caption
{
    return shift->value(@_);
}

1;

__DATA__

<!--html{
<input type="<%=$self->type%>" value="<%=$self->caption%>" <%=$self->render_atts%>
<% if( $self->accesskey) {%> accesskey="<%=$self->accesskey%>" <% } %>
/>
}html-->

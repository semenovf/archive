package Webject::Button;
use base 'Webject';
use strict;

sub REGULAR { 'button' }
sub SUBMIT  { 'submit' }
sub RESET   { 'reset' }

sub ctor
{
    my $self = shift;
    $self->SUPER::ctor();
    $self->set_attributes( qw(value type accesskey) );
    $self->caption('Ok');
    $self->type(SUBMIT);
}

# synonim to 'value' method
sub caption
{
    return shift->value(@_);
}

1;

__DATA__

<!--html{
<input <%=$self->render_atts%> />
}html-->

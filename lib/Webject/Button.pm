package Webject::Button;
use base 'Webject';
use strict;

sub REGULAR { 'button' }
sub SUBMIT  { 'submit' }
sub RESET   { 'reset' }

__PACKAGE__->mk_accessors(@rw_accessors, @rw_atts);


sub ctor
{
    my $self = shift;
    $self->caption('Ok');
    $self->type(SUBMIT);
    $self->set_attributes( qw(value type accesskey) );
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

package Webject::TextBox;
use base 'Webject';

sub REGULAR      {'text'}
sub AUTOCOMPLETE {1}
sub PASSWORD     {'password'}


sub ctor
{
    my $self = shift;
    $self->SUPER::ctor();
    $self->set_attributes( qw(name type value maxlength) );
    $self->type(REGULAR); 
}

1;

__DATA__

<!--html{
<input <%=$self->render_atts%> />
}html-->

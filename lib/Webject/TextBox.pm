package Webject::TextBox;
use base 'Webject';

sub REGULAR      {'text'}
sub AUTOCOMPLETE {1}
sub PASSWORD     {'password'}

my @rw_accessors = qw(
    name
    type
    value
    maxlength
);

__PACKAGE__->mk_accessors(@rw_accessors);

sub new {
    my $class = shift;
    my $self = bless $class->SUPER::new(@_), $class;
    $self->type(REGULAR); 
    return $self;
}

1;

__DATA__

<!--html{
<input type="<%=$self->type%>" <%=$self->render_atts %>
    <%=stringify_att( $self, 'name' )%>
    <%=stringify_att( $self, 'maxlength' )%>
    <%=stringify_att( $self, 'value' )%> />
}html-->

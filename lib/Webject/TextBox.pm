package Webject::TextBox;
use base 'Webject';

sub REGULAR      {0}
sub AUTOCOMPLETE {1}
sub PASSWORD     {2}

my @rw_accessors = qw(
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

sub stringify_type
{
    return 'password' if $_[0]->type == PASSWORD;
    return 'text';
}


1;

__DATA__

<!--html{
<input type="<%=$self->stringify_type%>" <%=$self->render_atts %>
    <%=stringify_att( $self, 'maxlength' ) %>
    <%=stringify_att( $self, 'value' ) %> />
}html-->

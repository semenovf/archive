package Webject::TextBox;
use base 'Webject';

sub REGULAR      {'text'}
sub AUTOCOMPLETE {1}
sub PASSWORD     {'password'}

my @rw_atts = qw(
    name
    type
    value
    maxlength
);

my @rw_accessors = qw(
);

__PACKAGE__->mk_accessors(@rw_accessors, @rw_atts);

sub new {
    my $class = shift;
    my $self = bless $class->SUPER::new(@_), $class;
    $self->type(REGULAR); 
    return $self;
}

1;

__DATA__

<!--html{
<input type="<%=$self->type%>" <%=$self->render_atts(\@Webject::TextBox::rw_atts) %> />
}html-->

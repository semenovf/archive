package Webject::Label;
use base 'Webject';

my @rw_atts = qw(
    for
);

my @rw_accessors = qw();

__PACKAGE__->mk_accessors(@rw_accessors, @rw_atts);

#sub new {
#    my $class = shift;
#    my $self = $class->SUPER::new(@_);
#    return bless $self, $class;
#}

1;

__DATA__

<!--html{
<label <%=$self->render_atts(\@Webject::Label::rw_atts)%>>
    <%=$self->render_children %>
</label>
}html-->



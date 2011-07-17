package Webject::HRef;
use base Webject;
use strict;

__PACKAGE__->mk_accessors(qw(href));

sub new {
    my $class = shift;
    my $self = $class->SUPER::new(@_);
    $self->href('#');
    return bless $self, $class;
}

1;


__DATA__

<!--html{
<a href="<%=$self->href%>" <%=$self->render_atts %> >
    <%=$self->render_children %>
</a>
}html-->

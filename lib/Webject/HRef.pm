package Webject::HRef;
use base Webject;
use strict;


my @rw_atts = qw(
    href
);

my @rw_accessors = qw();

__PACKAGE__->mk_accessors(@rw_accessors, @rw_atts);

sub new {
    my $class = shift;
    my $self = $class->SUPER::new(@_);
    $self->href('#');
    return bless $self, $class;
}

1;


__DATA__

<!--html{
<a <%=$self->render_atts(\@Webject::HRef::rw_atts) %> >
    <%=$self->render_children %>
</a>
}html-->

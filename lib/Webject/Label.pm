package Webject::Label;
use base 'Webject';

sub BEFORE { 0 }
sub AFTER  { 1 }

#my @rw_atts = qw(
#);

my @rw_accessors = qw(
    value
    where
);

#__PACKAGE__->mk_accessors(@rw_atts);
__PACKAGE__->mk_accessors(@rw_accessors);

sub new {
    my $class = shift;
    my $self = bless $class->SUPER::new(@_), $class;
    $self->value('');
    $self->where(BEFORE);
    return $self;
}

1;

__DATA__

<!--html{
<label <%=$self->render_atts %> >
    <%=encode $self->value if $self->where == Webject::Label::BEFORE %>
    <%=$self->render_children %>
    <%=encode $self->value if $self->where == Webject::Label::AFTER %>
</label>
}html-->



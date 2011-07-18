package Webject::Label;
use base 'Webject';

sub BEFORE { 'before' }
sub AFTER  { 'after' }

my @rw_accessors = qw(
    value
    where
    for
);

__PACKAGE__->mk_accessors(@rw_accessors);


sub new {
    my $class = shift;
    my $self = $class->SUPER::new(@_);
    $self->value('');
    $self->where(BEFORE);
    return bless $self, $class;
}

1;

__DATA__

<!--html{
<label <%=$self->render_atts %>
    <%=$self->for ? 'for="' . $self->for .'"' : '' %>
    >
    <%=$self->value if $self->where eq Webject::Label::BEFORE%>
    <%=$self->render_children %>
    <%=$self->value if $self->where eq Webject::Label::AFTER%>
</label>
}html-->



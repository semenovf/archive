package Webject::Native;
use Webject::Media::Functions;
use base 'Webject';

my @rw_accessors = qw(
    tag
);

__PACKAGE__->mk_accessors(@rw_accessors);

sub render_atts
{
    my $self = shift;
    my $t = '';
    if( defined $self->{'atts'} ) {
        $t .= ' ';
        $t = stringify_atts(%{$self->{'atts'}});
    }

    return ($self->SUPER::render_atts() . $t);
}


sub add_atts
{
    my ($self, %atts) = @_;
    $self->{'atts'} = {} unless defined $self->{'atts'};
    
    while ( my($k, $v) = each %atts ) {
        $self->{'atts'}->{$k} = $v;
    }
    return $self;
}

1;

__DATA__

<!--html{
<<%=$self->tag%> <%=$self->render_atts %>>
    <%=$self->render_children %>
</<%=$self->tag%>>
}html-->


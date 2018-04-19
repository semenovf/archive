package Webject::Table;
use Webject::Table::Section;
use base 'Webject';

# section types
sub BODY   {'body'}
sub HEADER {'header'}
sub FOOTER {'footer'}

1;

__DATA__

<!--html{
<table <%=$self->render_atts%>>
    <%= $self->render_children %>
</table>
}html-->


sub render
{
    my $self = shift or die;
    my $attrs = $self->attrs;

    my $html = '<table';
    $html .= ' ' . $attrs if $attrs;
    $html .= ' cellpadding="' . $self->cellpadding . '"'  if defined $self->cellpadding;
    $html .= ' cellspacing="' . $self->cellspacing . '"'  if defined $self->cellspacing;
    $html .= ' border="' . $self->cellspacing . '"' if defined $self->border;
    $html .= '>';

    $html .= $self->{'thead'}->render if $self->{'thead'};
    $html .= $self->{'tfoot'}->render if $self->{'tfoot'};
    $html .= $self->{'tbody'}->render if $self->{'tbody'};

    $html .= '</table>';
    return $html;
}

1;

package Webject::Table::Cell;
use base 'Webject';

1;

__DATA__

<!--html{
<td <%= $self->render_atts %>>
    <%= $self->render_children %>
</td>
}html-->

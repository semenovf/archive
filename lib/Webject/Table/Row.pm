package Webject::Table::Row;
use Webject::Table::Cell;
use base 'Webject';

sub new {
    my $class = shift;
    my $self = bless $class->SUPER::new(@_), $class;
#    $self->{-cells} = [];
    return $self;
}

sub cell_at
{
    my ($self, $index) = @_;
    return $self->_child_at($index);
}

sub new_cell
{
    my $self = shift or die;
    my $cell = Webject::Table::Cell->new;
    $self->add($cell);
    return $cell;
}


sub add
{
    my $self = shift or die;
    $self->SUPER::add(@_);
    return $self;
}


sub cells
{
    return wantarray ? @{$_[0]->{-cells}} : $_[0]->{-cells};
}


sub count
{
    return scalar @{$_[0]->{-cells}};
}

1;

__DATA__

<!--html{
<tr <%= $self->render_atts %>>
    <%= $self->render_children %>
</tr>
}html-->


#$oddeven = $i % 2 ? 'odd' : 'even';

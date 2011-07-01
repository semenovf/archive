package Webject::Table::Row;
use Webject::Table::Cell;
use base 'Webject';

sub new {
    my $class = shift;
    my $self = bless $class->SUPER::new(@_), $class;
    $self->{-cells} = [];
    return $self;
}

sub get_cell_content
{
    my $self = shift or die;
    my $col = shift;

    return undef unless defined $col;
    return undef if( $col < 0 || $col >= scalar(@{$self->{-cells}}) );
    return $self->{-cells}->[$col];
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
    my $cell = Webject::Table::Cell->new;
    $cell->SUPER::add(@_);

    push @{$self->{-cells}}, $cell;
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
<tr <%=$self->render_atts %>>
    <%=
        my $cells = '';
        foreach( @{$self->cells} ) {
            $cells .= $_->render;
        }
        $cells;
    %>
</tr>
}html-->


#$oddeven = $i % 2 ? 'odd' : 'even';

package Webject::Table::Section;
use Webject::Table;
use Webject::Table::Row;
use base 'Webject';

my @rw_accessors = qw(
    type
);

__PACKAGE__->mk_accessors(@rw_accessors);

sub new {
    my $class = shift;
    my $self = bless $class->SUPER::new(@_), $class;
#    $self->{-rows} = [];
    $self->type(Webject::Table::BODY);
    return $self;
}

sub new_row
{
    my $self = shift or die;
    my $row = Webject::Table::Row->new;
    $self->add($row);
    return $row;
}


sub add
{
    my $self = shift or die;
#    my $row = Webject::Table::Row->new;
    $self->SUPER::add(@_);

 #   push @{$self->{-rows}}, $row;
    return $self;
}


sub row_at
{
    my ($self, $index) = @_;
    return $self->_child_at($index);
}

sub rows
{
    return $_[0]->_children;
}

sub count
{
    return $_[0]->_children_count;
}

1;

__DATA__

<!--html{
<% if( $self->count ) {
    my $tag = (($self->type == Webject::Table::BODY) ? 'tbody'
        : (($self->type == Webject::Table::HEADER) ? 'thead' : 'tfoot'));
%>
<<%=$tag%> <%= $self->render_atts %>>
    <%= $self->render_children %>
</<%=$tag%>>
<% } %>
}html-->


# $oddeven = $i % 2 ? 'odd' : 'even';

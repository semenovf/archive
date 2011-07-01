package Webject::Table::Row;
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
    $self->{-rows} = [];
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
    my $row = Webject::Table::Row->new;
    $row->SUPER::add(@_);

    push @{$self->{-rows}}, $row;
    return $self;
}


sub rows
{
    return wantarray ? @{$_[0]->{-rows}} : $_[0]->{-rows};
}

sub count
{
    return scalar @{$_[0]->{-rows}};
}

1;

__DATA__

<!--html{
<% if( $self->count ) {
    my $tag = $self->type == Webject::Table::BODY ? 'tbody'
        : $self->type == Webject::Table::HEADER ? 'thead' : 'tfoot';
%>
<<%=$tag%> <% $self->render_atts %>>
    <%=
        my $rows = '';
        foreach( @{$self->rows} ) {
            $rows .= $_->render;
        }
        $rows;
    %>
</<%=$tag%>>
<% } %>
}html-->


# $oddeven = $i % 2 ? 'odd' : 'even';

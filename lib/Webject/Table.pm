package Webject::Table;
package Webject::Table::Section;
#package Webject::Table::Footer;
#package Webject::Table::Body;
use base 'Webject';

# section types
sub BODY   {0}
sub HEADER {1}
sub FOOTER {2}

sub REGULAR      {0}
sub AUTOCOMPLETE {1}
sub PASSWORD     {2}

my @rw_accessors = qw(
);

__PACKAGE__->mk_accessors(@rw_accessors);

sub new {
    my $class = shift;
    my $self = $class->SUPER::new(@_);
    $self->{'header'} = undef;
    $self->{'body'}   = undef;
    $self->{'footer'} = undef;
    return bless $self, $class;
}


sub header
{
    $_[0]->{'header'} = Webject::Table::Section->new->type(HEADER)
        unless defined $_[0]->{'header'};
    return $_[0]->{'header'};
}

sub body
{
    $_[0]->{'body'} = Webject::Table::Section->new->type(BODY)
        unless defined $_[0]->{'body'};
    return $_[0]->{'body'};
}

sub footer
{
    $_[0]->{'footer'} = Webject::Table::Section->new->type(FOOTER)
        unless defined $_[0]->{'footer'};
    return $_[0]->{'footer'};
}


1;

__DATA__

<!--html{
<table <%=$self->render_atts %>
    <%= $self->header->render %>
    <%= $self->body->render %>
    <%= $self->footer->render %>
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

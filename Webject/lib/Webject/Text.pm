package Webject::Text;
use base 'Webject';

__PACKAGE__->mk_accessors(qw(value));


sub ctor
{
    my $self = shift;
    $self->SUPER::ctor();
    $self->value('');
}

1;

__DATA__

<!--html{
<%=$self->value %>
}html-->


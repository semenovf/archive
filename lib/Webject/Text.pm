package Webject::Text;
use base 'Webject';

my @rw_accessors = qw(
    value
);

__PACKAGE__->mk_accessors(@rw_accessors);

sub new {
    my $class = shift;
    my $self = bless $class->SUPER::new(@_), $class;
    $self->value('');
    return $self;
}

1;

__DATA__

<!--html{
<%=$self->value %>
}html-->


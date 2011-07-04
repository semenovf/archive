package Webject::Button;
use base 'Webject';
use strict;

sub REGULAR { 0 }
sub SUBMIT  { 1 }
sub RESET   { 2 }

my @rw_accessors = qw(
    caption
    type
);

__PACKAGE__->mk_accessors(@rw_accessors);


sub new
{
    my $class = shift;
    my $self = $class->SUPER::new(@_);
    $self->caption('ok');
    $self->type(SUBMIT);
    return bless $self, $class;
}

sub stringify_type
{
    my $self = shift;
    return 'submit' if $self->type == SUBMIT;
    return 'reset'  if $self->type == RESET;
    return 'button';
}

1;

__DATA__

<!--html{
<input type="<%=$self->stringify_type%>" value="<%=$self->caption%>" <%=$self->render_atts %> />
}html-->

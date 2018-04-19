package JQ::Validator;
use strict;

=item new
    my $validator = JQ::Validator->new;
=cut

sub new
{
    my $class = shift;
    my $self = bless {}, $class;
    return $self;
}

=item new
    my $validator = JQ::Validator->new;
=cut

sub entry($$)
{
    my ($self, $entry_id) = @_;
    $self->{$entry_id} = JQ::Validator::Entry->new unless defined $self->{$entry_id};
    return $self->{$entry_id};
}


package JQ::Validator::Entry;
use base 'JQ::Errorable';
use strict;

sub new
{
    my $class = shift;
    my $self = $class->SUPER::makeErrorable();
    return bless $self, $class;
}

sub required($$)
{
    my ($self, $b) = @_;
    $self->{'required'} = $b;
    return $self;
}


sub default($$)
{
    my ($self, $v) = @_;
    $self->{'default'} = $v;
    return $self;
}

sub regexp($$)
{
    my ($self, $rx) = @_;
    $self->{'regexp'} = $rx;
    return $self;
}

sub type($$)
{
    my ($self, $t) = @_;
    $self->{'type'} = $t;
    return $self;
}

sub is_valid($$)
{
    my ($self, $v) = @_;

    unless ( defined $v ) {
        if
    }

    my $b = 1;
    $b = defined

}

1;

package Webject::Clipboard;

sub new { return bless [], $_[0]; }

sub push
{
    my $self = shift;
    push @{$self}, @_;
    return $self;
}

sub fetch
{
    my $self = shift;
    return wantarray ? @{$self} : $self;
}

sub clear
{
    my $self = shift;
    splice @{$self};
    return $self;
}

1;
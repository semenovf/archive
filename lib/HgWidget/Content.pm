package HgWidget::Expander;
use strict;

sub new
{
    my $class = shift or die;
    my $id = shift or die;
    my $self = {
        id => $id
    };
    return bless $self, $class;
}

1;
package JQ::Web::SearchEngineQuest;
use strict;

use base qw(JQ::Web::Quest);


sub new {
    my $class = shift;
    my $self = bless $class->SUPER::new(@_), $class;
    return $self;
}



1;

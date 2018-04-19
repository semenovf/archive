package DBI::DDI::SQL;
use Carp;
use strict;


# Launched by JQ::DBI::ddi
sub spec_for_deploy
{
    croak __PACKAGE__.'::spec_for_deploy() must be implemented';
}


sub spec_for_recall
{
    croak __PACKAGE__.'::spec_for_recall(): must be implemented';
}

1;

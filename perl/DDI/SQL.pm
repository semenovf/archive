package JQ::DDI::SQL;
use strict;


# Launched by JQ::DBI::ddi
sub specForDeploy
{
    die __PACKAGE__.'::specForDeploy() must be implemented';
}


sub specForRecall
{
    die __PACKAGE__.'::specForRecall(): must be implemented';
}

1;

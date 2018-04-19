package JQ::Web::Quest::Expert::Job::Const;
use strict;
use base 'Exporter';
our @EXPORT = qw(
    JOB_BRANCH_ALL
    JOB_BRANCH_CHIEF
    JOB_BRANCH_IT
);

sub JOB_BRANCH_ALL     {0}
sub JOB_BRANCH_CHIEF   {1}
sub JOB_BRANCH_IT      {2}

1;

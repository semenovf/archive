use 5.006;
use strict;
use warnings;
use ExtUtils::MakeMaker;

WriteMakefile(
    NAME                => 'DBI::DDI',
    AUTHOR              => q{Fedor Semenov <fedor.v.semenov@gmail.com>},
    VERSION_FROM        => 'lib/DBI/DDI.pm',
    ABSTRACT_FROM       => 'lib/DBI/DDI.pm',
    ($ExtUtils::MakeMaker::VERSION >= 6.3002
      ? ('LICENSE'=> 'perl')
      : ()),
    PL_FILES            => {},
    PREREQ_PM => {
        'Test::More' => 0,
    },
    dist                => { COMPRESS => 'gzip -9f', SUFFIX => 'gz', },
    clean               => { FILES => 'DBI-DDI-*' },
);

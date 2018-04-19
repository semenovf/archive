use 5.006;
use strict;
use warnings;
use ExtUtils::MakeMaker;

WriteMakefile(
    NAME                => 'Class::Accessor::Validated',
    AUTHOR              => q{Fedor Semenov <fedor.v.semenov@gmail.com>},
    VERSION_FROM        => 'lib/Class/Accessor/Validated.pm',
    LICENSE             => 'perl',
    PL_FILES            => {},
    PREREQ_PM => {
        'Test::More' => 0,
    },
    dist                => { COMPRESS => 'gzip -9f', SUFFIX => 'gz', },
    clean               => { FILES => 'Class-Accessor-Validated-*' },
);

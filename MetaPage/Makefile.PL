use 5.006;
use strict;
use warnings;
use ExtUtils::MakeMaker;

my $EUMM_VERSION = $ExtUtils::MakeMaker::VERSION;
$EUMM_VERSION =~ s/[^\.0-9]//g;

WriteMakefile(
<<<<<<< HEAD
    NAME                => 'Validator::Var',
    AUTHOR              => q{Fedor Semenov <fedor.v.semenov@gmail.com>},
    VERSION_FROM        => 'lib/Validator/Var.pm',
    ABSTRACT_FROM       => 'lib/Validator/Var.pm',
=======
    NAME                => 'MetaPage',
    AUTHOR              => q{Fedor Semenov <fedor.v.semenov@gmail.com>},
    VERSION_FROM        => 'lib/MetaPage.pm',
    ABSTRACT_FROM       => 'lib/MetaPage.pm',
>>>>>>> MetaPage/master
    ($EUMM_VERSION >= 6.3002
      ? ('LICENSE'=> 'perl')
      : ()),
    PL_FILES            => {},
    PREREQ_PM => {
        'Test::More' => 0,
<<<<<<< HEAD
	'Scalar::Util' => 0,
    },
    dist                => { COMPRESS => 'gzip -9f', SUFFIX => 'gz', },
    clean               => { FILES => 'Validator-Var-*' },
=======
    },
    dist                => { COMPRESS => 'gzip -9f', SUFFIX => 'gz', },
    clean               => { FILES => 'MetaPage-*' },
>>>>>>> MetaPage/master
);

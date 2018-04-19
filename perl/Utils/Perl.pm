package JQ::Utils::Perl;
use Config;
use strict;
use base 'Exporter';
our @EXPORT_OK = qw(perl_interpreter);

sub perl_interpreter
{
    my $perl  = $Config{perlpath};

    if ($^O ne 'VMS') {
        $perl .= $Config{_exe} unless $perl =~ m/$Config{_exe}$/i;
    }

    return $perl;
}

1;

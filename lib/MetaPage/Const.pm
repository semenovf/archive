package MetaPage::Const;
use strict;
use base 'Exporter';
our @EXPORT = qw(_INCLUDES_ _TEXT_ _ALIASES_ _ROOT_);

sub _INCLUDES_  {'.includes'}
sub _TEXT_      {'.text'}
sub _ALIASES_   {'.aliases'}

sub _ROOT_      {'metapage'}

1;
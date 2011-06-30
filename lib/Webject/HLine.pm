package Webject::HLine;
use base Webject;

use 5.006;
use strict;
use warnings;

sub render { my $self = $_[0]; $self->render_file(*DATA); }

1;

__DATA__
<hr <% my $hello = "Hello from Web::Webject::HLine" %> />
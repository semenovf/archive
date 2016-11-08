package gbspp::cli::router;
use strict;
use warnings;

package gbspp::cli::router::action;
use strict;
use warnings;

sub new
{
    my $class = shift or die;
    my $self = bless {
        aliases => [] # action aliases
    }, $class;
    return $self;
}

sub contains
{
    die unless @_ == 2;
    my ($self, $action) = @_;

    foreach my $a (@{$self->{aliases}}) {
        return 1 if $a eq $action;
    }
    return 0;
}

package gbspp::cli::router::option;
use strict;
use warnings;

sub new
{
    my $class = shift or die;
    my $self = bless {
          name          => undef
        , stash_name    => undef
        , default_value => undef
        , variants      => []
        , value         => undef
    }, $class;
    return $self;
}

sub name
{
    my $self = shift or die;
    return $self->{name};
}

sub stash_name
{
    my $self = shift or die;
    return $self->{stash_name};
}

sub set_default_value
{
    die unless @_ == 2;
    my ($self, $value) = @_;
    $self->{default_value} = $value;
}

sub add_variant
{
    die unless @_ == 2;
    my ($self, $value) = @_;
    push(@{$self->{variants}}, $value);
}

sub set_value
{
    die unless @_ == 2;
    my ($self, $value) = @_;
    push(@{$self->{variants}}, $value);
}

package gbspp::cli::router::route;
use strict;
use warnings;

sub new
{
    my $class = shift or die;
    my $self = bless {
        actions = [] # action aliases
        options     = require("pfs.map"):new()
        , _h        = nil -- handler
        , _args     = require("pfs.array"):new() -- free arguments
        , _ropts    = nil   -- filled by run() method, contains pairs {optname, optarg}
        , _continue = false -- if `true' stops after successful matching
    }, $class;
    return $self;
}

package gbspp::cli::router;

sub new
{
    my $class = shift or die;
    my $self = bless {}, $class;
    return $self;
}


1;

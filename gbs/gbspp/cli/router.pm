package gbspp::cli::router;
use gbspp::sub qw(required);
use gbspp::type::bool;
use gbspp::type::numeric;
use gbspp::type::string;
use strict;
use warnings;

############################################
#package gbspp::cli::router::domain;
#use strict;
#use warnings;
#
#sub new
#{
#    my $class = shift or die;
#    my $self = bless {
#        aliases => [] # domain aliases
#    }, $class;
#    return $self;
#}
#
##
## $domain->contains($str)
##
#sub contains
#{
#    die unless @_ == 2;
#    my ($self, $domain) = @_;
#
#    foreach my $a (@{$self->{aliases}}) {
#        return 1 if $a eq $domain;
#    }
#    return 0;
#}
#
##
## $domain->add(@aliases)
## $domain->add(\@aliases)
## $domain->add($alias)
##
#sub add
#{
#    my $self = shift or die;
#    
#    (@_ == 1 and ref($_[0]) eq '')      and do { push(@{$self->{aliases}}, $_[0]); return; };
#    (@_ == 1 and ref($_[0]) eq 'ARRAY') and do { push(@{$self->{aliases}}, @{$_[0]}); return; };
#    (@_ > 1)                            and do { push(@{$self->{aliases}}, @_); return; };
#}

############################################
#package gbspp::cli::router::option;
#use strict;
#use warnings;
#
#sub new
#{
#    my $class = shift or die;
#    my $self = bless {
#          name          => undef
#        , stash_name    => undef
#        , default_value => undef
#        , variants      => []
#        , value         => undef
#    }, $class;
#    return $self;
#}
#
#sub name
#{
#    my $self = shift or die;
#    return $self->{name};
#}
#
#sub set_name
#{
#    die unless @_ == 2;
#    my ($self, $value) = @_;
#    $self->{name} = $value;
#}
#
#
#sub stash_name
#{
#    my $self = shift or die;
#    return $self->{stash_name};
#}
#
#sub set_stash_name
#{
#    die unless @_ == 2;
#    my ($self, $value) = @_;
#    $self->{stash_name} = $value;
#}
#
#sub set_default_value
#{
#    die unless @_ == 2;
#    my ($self, $value) = @_;
#    $self->{default_value} = $value;
#}
#
#sub add_variant
#{
#    die unless @_ == 2;
#    my ($self, $value) = @_;
#    push(@{$self->{variants}}, $value);
#}
#
#sub set_value
#{
#    die unless @_ == 2;
#    my ($self, $value) = @_;
#    push(@{$self->{variants}}, $value);
#}

############################################
package gbspp::cli::router::dummy_route;
use gbspp::sub qw(required);
use strict;
use warnings;

sub new
{
    my $class = required(\@_);
    return bless {} , $class;
}

sub d  { return $_[0]; }
sub to { return 0; }

############################################
package gbspp::cli::router::route;
use Carp;
use gbspp::sub qw(required);
use base 'gbspp::cli::router::dummy_route';
use strict;
use warnings;

sub new
{
    my $class = required(\@_);
    my $cli   = required(\@_);

    my $self = $class->SUPER::new;
    $self->{cli} = $cli;
    
#    my $self = bless {
#          domains => [] # domain aliases
#        , options => {}
##        , _h        = nil -- handler
##        , _args     = require("pfs.array"):new() -- free arguments
##        , _ropts    = nil   -- filled by run() method, contains pairs {optname, optarg}
##        , _continue = false -- if `true' stops after successful matching
#        , handler => undef
#    }, $class;
    return $self;
}

#
# $self->to(\&handler)
#
sub to
{
    my $self  = required(\@_);
    my $sub   = required(\@_);
    
    croak "Subroutine expected for route handler" unless ref($sub) eq 'CODE';
    return $sub->();
}

#
# $self->d(@aliases)
# $self->d(\@aliases)
# $self->d($alias)
#
sub d
{
    my $self  = required(\@_);
    my $alias = required(\@_);

    my @aliases = ();
    
    if (@_ > 1) {
        push(@aliases, $alias);
        push(@aliases, @_);
    } else {
        if (ref($alias) eq 'ARRAY') {
            push(@aliases, @$alias);
        } else {
            push(@aliases, $alias);
        }
    }

    foreach my $a (@aliases) {
        return $self if $a eq $self->{cli}->domain;
    }
    
    return gbspp::cli::router::dummy_route->new;
}

#
# Common signatures for option types
#
# 1. $self->OPTTYPE($optname)
# 2. $self->OPTTYPE($optname, $stashname)
# 3. $self->OPTTYPE($optname, $stashname => DEFAULT_VALUE)
# 4. $self->OPTTYPE($optname, $stashname => {-default => DEFAULT_VALUE [, -variants => @VARIANTS] })
#
# OPTTYPE is one of rule: 'b', 's' or 'n'
#

#
# Return 'canonical' respresentation of paramaters as array in format
#   ($optname, $stash_name, $default_value, \@variants)
#
# Throws exception (died) if $optname is 'undef'.
#
sub _canonical_parms
{
    #my ($optname, $stash_name, $default_value, $variants) = (undef, undef, undef, undef);
    my ($optname, $stash_name, $extra) = @_;
    my ($default_value, $variants) = (undef, undef);
    
    die 'Bad option name' unless (defined($optname) and ref($optname) eq '');
    die 'Bad stash name' if (defined($stash_name) and ref($optname) ne '');
    die 'Bad extra parameters' if (defined($extra) and (ref($extra) eq '' or ref($extra) eq 'HASH'));
    
    if (defined($extra)) {
        if (ref($extra) eq '') {
            $default_value = $extra;
        } elsif (ref($extra) eq 'HASH') {
            $default_value = $extra->{-default};
            $variants = $extra->{-variants};
            
            die 'Bad variants' if (defined($variants) and ref($variants) ne 'HASH');
        }
    }
        
    return ($optname, $stash_name, $default_value, $variants);
}

#
# Sets policy for boolean option
#
# @note '-variants' is ignored
#
sub b
{
    my $self = shift or die;
    my ($optname, $stash_name, $default_value, $variants) = _canonical_parms(@_);
    
    if (@_ > 0 and ref($_[0]) eq '') {
        my $option = gbspp::cli::router::option->new;
        $self->{handler} = $_[0];
    }
    
    return $self;
}

#
# Sets policy for string option
#
sub s
{
#    return $self;
}

############################################
package gbspp::cli::router;

sub new
{
    my $class = required(\@_);
    my $cli   = required(\@_);
    
    my $self = bless {
        cli => $cli
    }, $class;
    
    return $self;
}

sub r
{
    my $self = required(\@_);
    return gbspp::cli::router::route->new($self->{cli});
}

1;

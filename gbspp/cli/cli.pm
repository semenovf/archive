package gbspp::cli::cli;

use File::Basename qw(basename dirname);
use Cwd 'abs_path';
use strict;
use warnings;

sub new 
{
    my $class = shift or die;
    my $program = defined $_[0] ? $_[0] : $0;
    
    $program = abs_path($program);
    
    my $self = {
        path     => $program                     # full path to executable/script (i.e. `/path/to/bootstrap.pl')
      , name     => basename($program)           # file name (i.e. `bootstrap.pl')
      , corename => basename($program, ('.pl'))  # file name excluding suffixes (i.e. `bootstrap')
      , dir      => dirname($program)            # directory path (i.e. `/path/to')
      , has_domain => 0                          # first option is a free argument
      , options  => {}
      , args     => []
    };

    return bless $self, $class;
}

sub parse_array
{
    my $self = shift or die;
    my $argv = scalar(@_) == 1 ? $_[0] : \@_;
    my $argc = scalar(@$argv);
    
    my $options = $self->{options};
    my $args    = $self->{args};
    
    if ($argc > 0) {
        my $i = 0;
        
        #
        # Check action
        #
        if ($argv->[0]) {
            if (!($argv->[0] =~ /^\-{1,2}/)) {
                $self->{has_domain} = 1;
                push @{$args}, $argv->[0];
                ++$i;
            }
        }
        
        for (; $i < $argc; ++$i) {
            my $optname = undef;
            my $arg     = undef;

            if ($argv->[$i] =~ /^\-{1,2}([\w\d_][\w\d_-]*)=(.+)$/) {
                $optname = $1;
                $arg     = $2;
            } elsif ($argv->[$i] =~ /^\-{1,2}([\w\d_][\w\d_-]*)$/) {
                $optname = $1;
                $arg     = 1;
            } else {
                $arg = $argv->[$i];
            }
            
            #
            # remove double quotes from the begining and end of a string
            #
            $arg =~ s/^"(.*)"$/$1/; 
            
            if (defined $optname) {
                if (defined $options->{$optname}) {
                    if (ref($options->{$optname}) eq 'ARRAY') {
                        push(@{$options->{$optname}}, $arg);
                    } else {
                        my $oldarg = $options->{$optname};
                        $self->{options}->{$optname} = [];
                        $options = $self->{options};
                        push(@{$options->{$optname}}, $oldarg);
                        push(@{$options->{$optname}}, $arg);
                    }
                } else {
                    $options->{$optname} = $arg;
                }
            } else {
                push @{$args}, $arg;
            }
        }
    }
    
    return 1;
}

sub parse_ARGV
{
    my $self = shift or die;
    my $argc = $#ARGV >= 0 ? 1 + $#ARGV : 0;
    my $argv = $#ARGV >= 0 ? \@ARGV : undef;

    return $self->parse_array($argc, $argv);
}

sub has_domain
{
    my $self = shift or die;
    return $self->{has_domain};
}

sub domain
{
    my $self = shift or die;
    return $self->{has_domain} ? $self->{args}->[0] : undef;
}

sub has_option
{
    die unless @_ == 2;
    my ($self, $optname) = @_;
    
    return defined $self->{options}->{$optname};
}

sub arg
{
    die unless @_ == 2;
    my ($self, $index) = @_;
    
    my $count = scalar(@{$self->{args}});
    --$count if $self->has_domain;
    
    return undef unless $count > 0;
    return undef unless $index >= 0 && $index < $count;
    return $self->{args}->[$index + 1] if $self->has_domain;
    return $self->{args}->[$index];
}

sub nargs
{
    my $self = shift or die;
    
    my $result = scalar(@{$self->{args}});
    --$result if $self->has_domain;
    return $result;
}

sub optval
{
    die unless @_ == 2;
    my ($self, $optname) = @_;
    
    my $options = $self->{options};
    
    return undef unless defined $options->{$optname};
    
    if (wantarray) {
        return ($options->{$optname}) if ref($options->{$optname}) eq '';
        return @{$options->{$optname}} if ref($options->{$optname}) eq 'ARRAY';
    } else {
        return $options->{$optname} if ref($options->{$optname}) eq '';
        return $options->{$optname}->[scalar(@{$options->{$optname}}) - 1] if ref($options->{$optname}) eq 'ARRAY';
    }
    
    die q(Unexpected condition: ref($options->{$optname}) = ')
        . ref($options->{$optname}) . '\'';
    return undef;
}

1;

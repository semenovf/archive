package gbspp::shell;
use Term::ANSIColor;
use gbspp::Capture::Tiny 'capture';
use strict;
use warnings;

$Term::ANSIColor::EACHLINE = "\n";

my $info_color  = '';
my $debug_color = '';
my $warn_color  = '';
my $error_color = 'red';

sub run
{
    my ($command, @args) = @_;
    
    my @result = capture {
        system($command, @args);
    };

    return wantarray ? @result : $result[2];
}

sub set_info_color
{
    $info_color = $_[0];
}

sub set_debug_color
{
    $debug_color = $_[0];
}

sub set_warn_color
{
    $warn_color = $_[0];
}

sub set_error_color
{
    $error_color = $_[0];
}

sub info
{
    return colored([$info_color], @_);
}

sub debug
{
    return colored([$debug_color], @_);
}

sub warn
{
    return colored([$warn_color], @_);
}

sub error
{
    return colored([$error_color], @_);
}


1;

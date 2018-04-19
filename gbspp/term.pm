package gbspp::term;
use Term::ANSIColor;
use gbspp::Capture::Tiny 'capture';
use gbspp::os;
use strict;
use warnings;

$Term::ANSIColor::EACHLINE = "\n";

my $info_color  = '';
my $debug_color = 'bright_black';
my $warn_color  = 'magenta';
my $error_color = 'red';

sub run_command
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

sub term_size
{
    # Solution 1
    #
    # use Term::Screen; # ! Is not a core perl module
    # my $cols = cols();
    # my $rows = rows();

    # Solution 2
    #
    # use Term::ReadKey; # ! Is not a core perl module
    # my ($cols,$rows) = GetTerminalSize();

    # Solution 3 (unix)
    #
    if (gbspp::os::family eq 'unix') {
        my ($rows,$cols) = split(/\s+/,`/bin/stty size`);
        return ($rows,$cols) if defined($rows) and defined($cols);
    }
    
    # Solution 4 (unix)
    #
    # my $width = `tput cols`;
    
    return (25,80);
}

sub term_width
{
    my ($rows,$cols) = term_size;
    return $cols;
}

sub term_height
{
    my ($rows,$cols) = term_size;
    return $rows;
}

1;

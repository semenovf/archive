package gbspp::os;
use strict;
use warnings;

#
# @see http://alma.ch/perl/perloses.htm
# @see perldoc perlport
#

#require Exporter;
#our @ISA = qw(Exporter);
#our @EXPORT_OK = qw();
#

#
# gbspp::os::type
#
sub type
{
    return 'mswin'   if $^O =~ /MSWin/;
    return 'linux'   if $^O =~ /linux/;
    return 'bsd'     if $^O =~ /bsd$/;
    return 'macos'   if $^O =~ /darwin/;
    return 'solaris' if $^O =~ /solaris/;
    return 'dos'     if $^O =~ /^dos$/;
    return 'cygwin'  if $^O =~ /^cygwin/;
    
    return '';
}

sub family
{
    my $type = type;
    
    return 'unix' if ($type eq 'linux'
            || $type eq 'bsd'
            || $type eq 'macos'
            || $type eq 'solaris');
    
    return 'mswin' if ($type eq 'mswin' || $type eq 'cygwin');
    return 'dos'   if ($type eq 'dos');
}


1;

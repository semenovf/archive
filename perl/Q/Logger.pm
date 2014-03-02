package JQ::Logger;
use POSIX qw(strftime);
use Encode qw(encode);
require Exporter;
our @ISA = qw(Exporter);
our @EXPORT = qw(
    log_info
    log_debug
    log_warn
    log_error
    log_fatal
    log_info_f
    log_debug_f
    log_warn_f
    log_error_f
    log_fatal_f
    logging_debug
    logging_encoding);

my $_isLogDebug = 0;
my $_encoding   = undef;
my $LOG_INFO_DEV  = STDOUT;
my $LOG_WARN_DEV  = STDERR;
my $LOG_ERROR_DEV = STDERR;
my $LOG_FATAL_DEV = STDERR;
my $LOG_DEBUG_DEV = STDOUT;

sub __PREFIX { strftime '[%Y.%m.%d %H:%M:%S]', localtime }
sub __PREFIX_INFO  { __PREFIX.' INFO:  ' }
sub __PREFIX_WARN  { __PREFIX.' WARN:  ' }
sub __PREFIX_ERROR { __PREFIX.' ERROR: ' }
sub __PREFIX_FATAL { __PREFIX.' FATAL: ' }
sub __PREFIX_DEBUG { __PREFIX.' DEBUG: ' }

sub logging_debug
{
    return $_isLogDebug unless $_[0];
    $_isLogDebug = shift || 0;
}

sub logging_encoding
{
    my $enc = shift or die "Encoding for logger expected";
    die "Unknown or invalid encoding for logger" unless Encode::find_encoding($enc);
    $_encoding = $enc;
}

sub _Enc
{
    my @strlist = ();
    if( $_encoding ) {
        foreach my $str (@_) {
            my $s = $str;
            push @strlist, encode($_encoding, $s);
        }
    } else {
        foreach my $str (@_) {
            push @strlist, $str;
        }
    }
    return join('', @strlist);
}

#sub cout
#{
#    my $str = join('', @_)."\n";
#
#    if( $_encoding ) {
#        print encode($_encoding, $str);
#    } else {
#        print $str;
#    }
#}


sub log_debug_f { return unless $_isLogDebug; my $fmt = shift; print $LOG_DEBUG_DEV _Enc(sprintf __PREFIX_DEBUG.$fmt."\n", @_); }
sub log_info_f  { my $fmt = shift; print $LOG_INFO_DEV  _Enc(sprintf __PREFIX_INFO.$fmt."\n", @_); }
sub log_warn_f  { my $fmt = shift; print $LOG_WARN_DEV  _Enc(sprintf __PREFIX_WARN.$fmt."\n", @_); }
sub log_error_f { my $fmt = shift; print $LOG_ERROR_DEV _Enc(sprintf __PREFIX_ERROR.$fmt."\n", @_); }
sub log_fatal_f { my $fmt = shift; print $LOG_FATAL_DEV _Enc(sprintf __PREFIX_FATAL.$fmt."\n", @_); exit -1; }

sub log_debug { return unless $_isLogDebug; print $LOG_DEBUG_DEV _Enc(__PREFIX_DEBUG, @_, "\n"); }
sub log_info  { print $LOG_INFO_DEV  _Enc(__PREFIX_INFO, @_, "\n"); }
sub log_warn  { print $LOG_WARN_DEV  _Enc(__PREFIX_WARN, @_, "\n"); }
sub log_error { print $LOG_ERROR_DEV _Enc(__PREFIX_ERROR, @_, "\n"); }
sub log_fatal { print $LOG_FATAL_DEV _Enc(__PREFIX_FATAL, @_, "\n"); exit -1; }

1;

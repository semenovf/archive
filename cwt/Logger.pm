package cwt::Logger;

my $_isDebug = 0;

sub _print_with_prefix
{
    my $prefix = shift;
    my $msg = join( ' ', @_);
    chomp $msg;
    print "$prefix: $msg\n";
}

sub is_debug
{
    return $_isDebug unless $_[0];
    $_isDebug = shift || 0;
}

sub debug { 
    return unless $_isDebug;
    _print_with_prefix('DEBUG', @_);
}
sub info { _print_with_prefix('INFO', @_); }
sub warn { _print_with_prefix('WARN', @_); }
sub error { _print_with_prefix('ERROR', @_); }
sub fatal { _print_with_prefix('FATAL', @_); exit 1; }

1;
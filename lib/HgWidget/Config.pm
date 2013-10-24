package HgWidget::Config;
use strict;

my %_config = (
    theme => 'arctic'
);

#sub insert
#{
#    my ($key, $value) = @_;
#    $_config{$key} = $value;
#}

sub update
{
    shift; # skip self
    my ($key, $value) = @_;
    unless (defined $_config{$key}) {
        warn "Invalid configuration parameter: $key\n";
    } else {
        $_config{$key} = $value;
    }
}

#sub remove
#{
#    my $key = shift;
#    delete $_config{$key} if defined($key);
#}

sub get
{
    shift; # skip self
    my $key = shift;
    return $_config{$key};
}

sub import_styles
{
    shift; # skip self
    my @classes = @_;
    my @styles = ();
    
    push @styles, '/jqwidgets/styles/jqx.base.css';
    push @styles, '/jqwidgets/styles/jqx.' . $_config{theme} . '.css';

    return wantarray ? @styles : \@styles;
}

sub import_scripts
{
    shift; # skip self
    my @classes = @_;
    my @scripts = ();
    
    push @scripts, '/jquery/1.10.2/jquery.min.js';
    push @scripts, '/jqwidgets/jqxcore.js';
    
    foreach my $class (@classes) {
        $class eq 'Expander' and do { push @scripts, '/jqwidgets/jqxexpander.js'; next; };
    }
    
    return wantarray ? @scripts : \@scripts;
}

1;
package HgWidget::Widget;
use HgWidget::Global;
use Mo qw(is default chain);

my $_global = undef;

has id       => (is => 'rw', chain => 1, required => 1);
has width    => (is => 'rw', chain => 1, default => undef);
has height   => (is => 'rw', chain => 1, default => undef);
has disabled => (is => 'rw', chain => 1, default => 0);

sub global
{
    $_global = HgWidget::Global->new unless(defined $_global);
    return $_global;
}

sub _import_styles
{
    my $global = global();
    my $self = shift or die;
    [
          '/jqwidgets/styles/jqx.base.css'
        , '/jqwidgets/styles/jqx.' . $global->{theme} . '.css'
    ];
}

sub _import_scripts
{
    [
          '/jquery/1.10.2/jquery.min.js'
        , '/jqwidgets/jqxcore.js'
    ];
}

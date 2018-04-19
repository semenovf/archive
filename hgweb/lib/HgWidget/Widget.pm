package HgWidget::Widget;
use HgWidget::Global;
use HgWidget::Util;
use HgWidget::Layout;
use Mo qw(is default chain);

my $_global = undef;

has id       => (is => 'rw', chain => 1, required => 1);
has width    => (is => 'rw', chain => 1, default => undef);
has height   => (is => 'rw', chain => 1, default => undef);
has disabled => (is => 'rw', chain => 1, default => undef);
has layout   => (is => 'rw', chain => 1, default => HgWidget::Layout->new);

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

sub _js_attrs
{
	my $self = shift;
	jsJoinAttrs(
          jsAttrString('theme', $self->global->theme)
        , jsAttrString('width', $self->width)
        , jsAttrString('height', $self->height)
        , jsAttrBoolean('disabled', $self->disabled)
	);
}

sub _render_content
{
	my $self = shift;
	my $content = shift // '';
	
	my $rf = ref $content;
	
    unless ($rf) { # scalar
    	return $content;
    } elsif ($rf eq 'SCALAR') {
    	return $$rf;
    } elsif ($rf =~ /^HgWidget::/) {
    	return $content->render; 
    }
	
	'';
}

1;
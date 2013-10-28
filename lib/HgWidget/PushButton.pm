package HgWidget::PushButton;
use HgWidget::Util;
use Mo qw(is default chain required);
extends 'HgWidget::Widget';

has text   => (is => 'rw', chain => 1, default => 'PushButton');
has submit => (is => 'rw', chain => 1, default => 0);

sub _import_scripts
{
    [
        '/jqwidgets/jqxbuttons.js'
    ];
}

sub _js_attrs
{
	my $self = shift;
	jsJoinAttrs(
		$self->SUPER::_js_attrs
	);
}

sub controller
{
    my $self = shift;
    my $id = $self->id;
    my $attrs = $self->_js_attrs;
        
    my $r = <<"EndOfControllerData";
    \$("#$id").jqxButton({ $attrs });
EndOfControllerData

    $r;
}

sub render
{
    my $self = shift;
    my $id = $self->id;
    my $text = $self->text;
    my $type = $self->submit ? 'submit' : 'button';
    
    my $html = <<"EndOfViewData";
<input type="$type" value="$text" id='$id' />
EndOfViewData

    $html;
}

1;

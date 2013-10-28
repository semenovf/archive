package HgWidget::LineEdit;
use HgWidget::Util;
use Mo qw(is default chain required);
extends 'HgWidget::Widget';

#
# http://www.jqwidgets.com/jquery-widgets-demo/demos/jqxinput/defaultfunctionality.htm
#

has placeHolder => (is => 'rw', chain => 1, default => undef);
has source        => (is => 'rw', chain => 1, default => undef); # JS variable
#has animationType => (is => 'rw', chain => 1, default => 'none');

sub _import_scripts
{
    [
		'/jqwidgets/jqxinput.js'
    ];
}

sub _js_attrs
{
	my $self = shift;
	jsJoinAttrs(
		  $self->SUPER::_js_attrs
		, jsAttrVar('source', $self->source)
        , jsAttrString('placeHolder', $self->placeHolder)
	);
}

sub controller
{
    my $self = shift;
    my $id = $self->id;
    my $attrs = $self->_js_attrs;
    
    my $r = <<"EndOfControllerData";
    \$("#$id").jqxInput({ $attrs });
EndOfControllerData
    $r;
}

sub render
{
    my $self = shift;
    my $id = $self->id;
   
    my $html = <<"EndOfViewData";
    <input id="$id" type="text" />
EndOfViewData

    $html;
}

1;

package HgWidget::ComboBox;
use HgWidget::Util;
use Mo qw(is default chain required);
extends 'HgWidget::Widget';

#
# http://www.jqwidgets.com/jquery-widgets-demo/demos/jqxcombobox/defaultfunctionality.htm
#

has selectedIndex => (is => 'rw', chain => 1, default => undef);
has autoComplete  => (is => 'rw', chain => 1, default => undef);
has source        => (is => 'rw', chain => 1, default => undef); # JS variable
has animationType => (is => 'rw', chain => 1, default => 'none');

sub _import_scripts
{
    [
          '/jqwidgets/jqxscrollbar.js'
        , '/jqwidgets/jqxlistbox.js'
        , '/jqwidgets/jqxcombobox.js'
    ];
}

sub _js_attrs
{
	my $self = shift;
	jsJoinAttrs(
		$self->SUPER::_js_attrs
		, jsAttrBoolean('autoComplete', $self->autoComplete)
        , jsAttrVar('source', $self->source)
        , jsAttrString('animationType', $self->animationType)
        , jsAttrNumber('selectedIndex', $self->selectedIndex)
	);
}

sub controller
{
    my $self = shift;
    my $id = $self->id;
    my $attrs = $self->_js_attrs;
    
    my $r = <<"EndOfControllerData";
    \$("#$id").jqxComboBox({ $attrs });
EndOfControllerData
    $r;
}

sub render
{
    my $self = shift;
    my $id = $self->id;
   
    my $html = <<"EndOfViewData";
    <div id='$id'></div>
EndOfViewData

    $html;
}

1;

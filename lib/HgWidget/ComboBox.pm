package HgWidget::ComboBox;
use HgWidget::Util;
use Mo qw(is default chain required);
extends 'HgWidget::Widget';

#
# http://www.jqwidgets.com/jquery-widgets-demo/demos/jqxcombobox/defaultfunctionality.htm
#

has selectedIndex => (is => 'rw', chain => 1, default => undef);
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

sub controller
{
    my $self = shift;
    my $id = $self->id;
    my $attrs = jsJoinAttrs(
          jsAttrString('theme', $self->global->theme)
        , jsAttrVar('source', $self->source)
        , jsAttrString('animationType', $self->animationType)
        , jsAttrString('width', $self->width)
        , jsAttrString('height', $self->height)
        , jsAttrNumber('selectedIndex', $self->selectedIndex)
    );
    
    my $r = <<"EndOfControllerData";
    \$("#$id").jqxComboBox({ $attrs });
EndOfControllerData
    $r;
}

sub view
{
    my $self = shift;
    my $id = $self->id;
   
    my $html = <<"EndOfViewData";
    <div id='$id'></div>
EndOfViewData

    $html;
}

1;

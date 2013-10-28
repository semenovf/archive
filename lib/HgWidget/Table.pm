package HgWidget::Table;
use HgWidget::Util;
use Mo qw(is default chain required);
extends 'HgWidget::Widget';

#
# http://www.jqwidgets.com/jquery-widgets-demo/demos/jqxgrid/defaultfunctionality.htm
#
use constant FEATURE_BASE       => 0x0001;
use constant FEATURE_EDITABLE   => 0x0002;
use constant FEATURE_SELECTABLE => 0x0004;
use constant FEATURE_GROUPING   => 0x0008;
my $_features = FEATURE_BASE;

has columns  => (is => 'ro', chain => 1, default => []);
has features => (is => 'ro', chain => 1, default => FEATURE_BASE);

=item $table->setFeature($feature)
$feature := 'base' | 'editable' | 'selectable'
=cut

sub setFeature
{
	my $self = shift;
	my $feature = shift or return $self;
	my $ok = 0;
	
	$feature eq 'base'       and do { $ok = 1; $self->{features} |= FEATURE_BASE; $_features |= FEATURE_BASE; };
	$feature eq 'editable'   and do { $ok = 1; $self->{features} |= FEATURE_EDITABLE; $_features |= FEATURE_EDITABLE; };
	$feature eq 'selectable' and do { $ok = 1; $self->{features} |= FEATURE_SELECTABLE; $_features |= FEATURE_SELECTABLE; };
	$feature eq 'grouping'   and do { $ok = 1; $self->{features} |= FEATURE_GROUPING; $_features |= FEATURE_GROUPING; };
	
	warn "Bad feature to set for Table instance: $feature\n" unless $ok;
}

sub _import_scripts
{
	my $self = shift;
	my $r = [];
	
	push @$r, '/jqwidgets/jqxgrid.js'           if $_features & FEATURE_BASE;
	push @$r, '/jqwidgets/jqxgrid.edit.js'      if $_features & FEATURE_EDITABLE;
	push @$r, '/jqwidgets/jqxgrid.selection.js' if $_features & FEATURE_SELECTABLE;
	push @$r, '/jqwidgets/jqxgrid.grouping.js'  if $_features & FEATURE_GROUPING;

    $r;
}

sub _js_attrs
{
	my $self = shift;
	jsJoinAttrs(
		$self->SUPER::_js_attrs
#        , jsAttrVar('source', $self->source)
#        , jsAttrString('animationType', $self->animationType)
#        , jsAttrNumber('selectedIndex', $self->selectedIndex)
	);
}

sub controller
{
    my $self = shift;
    my $id = $self->id;
    my $attrs = $self->_js_attrs;
    
    my $r = <<"EndOfControllerData";    
	\$("#$id").jqxGrid({ $attrs
		, source: dataAdapter
		, columns: [
        	{ text: 'First Name', datafield: 'firstname', width: 100 },
            { text: 'Last Name', datafield: 'lastname', width: 100 },
            { text: 'Product', datafield: 'productname', width: 180 },
            { text: 'Quantity', datafield: 'quantity', width: 80, cellsalign: 'right' },
            { text: 'Unit Price', datafield: 'price', width: 90, cellsalign: 'right', cellsformat: 'c2' },
            { text: 'Total', datafield: 'total', width: 100, cellsalign: 'right', cellsformat: 'c2' }
		]
	});
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

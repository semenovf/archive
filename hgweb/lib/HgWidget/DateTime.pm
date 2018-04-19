package HgWidget::DateTime;
use HgWidget::Util;
use Mo qw(is default chain required);
extends 'HgWidget::Widget';

#
# http://www.jqwidgets.com/jquery-widgets-demo/demos/jqxdatetimeinput/defaultfunctionality.htm
#

=item date time input format

'd'-the day of the month
'dd'-the day of the month
'ddd'-the abbreviated name of the day of the week
'dddd'-the full name of the day of the week
'h'-the hour, using a 12-hour clock from 1 to 12
'hh'-the hour, using a 12-hour clock from 01 to 12
'H'-the hour, using a 24-hour clock from 0 to 23
'HH'-the hour, using a 24-hour clock from 00 to 23
'm'-the minute, from 0 through 59
'mm'-the minutes,from 00 though59
'M'-the month, from 1 through 12;
'MM'-the month, from 01 through 12
'MMM'-the abbreviated name of the month
'MMMM'-the full name of the month
's'-the second, from 0 through 59
'ss'-the second, from 00 through 59
't'-the first character of the AM/PM designator
'tt'-the AM/PM designator
'y'-the year, from 0 to 99
'yy'-the year, from 00 to 99
'yyy'-the year, with a minimum of three digits
'yyyy'-the year as a four-digit number
=cut

has formatString => (is => 'rw', chain => 1, default => 'dd/MM/yyyy');
has closeCalendarAfterSelection  => (is => 'rw', chain => 1, default => 1);
#has source        => (is => 'rw', chain => 1, default => undef); # JS variable
has animationType => (is => 'rw', chain => 1, default => 'none');

sub _import_scripts
{
    [
          '/jqwidgets/jqxdatetimeinput.js'
        , '/jqwidgets/jqxcalendar.js'
        , '/jqwidgets/jqxtooltip.js'
        , '/jqwidgets/globalization/globalize.js'
    ];
}

sub _js_attrs
{
	my $self = shift;
	jsJoinAttrs(
		$self->SUPER::_js_attrs
        , jsAttrString('formatString', $self->formatString)
        , jsAttrString('animationType', $self->animationType)
        , jsAttrString('closeCalendarAfterSelection', $self->closeCalendarAfterSelection)
	);
}

sub controller
{
    my $self = shift;
    my $id = $self->id;
    
    $self->width('100px') unless defined $self->width;
    
    my $attrs = $self->_js_attrs;
    
    my $r = <<"EndOfControllerData";
    \$("#$id").jqxDateTimeInput({ $attrs });
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

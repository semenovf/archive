package HgWidget::Expander;
use HgWidget::Util;
use Mo qw(is default chain required);
extends 'HgWidget::Widget';

has header   => (is => 'rw', chain => 1, default => 'Rename the header...');
has content  => (is => 'rw', chain => 1, default => 'Set the content...');
has expanded => (is => 'rw', chain => 1, default => 1);
has url      => (is => 'rw', chain => 1, default => undef);
has timeout  => (is => 'rw', chain => 1, default => '500');

sub _default_header_id
{
    my $id = shift or die;
    return $id . 'Header';
}

sub _default_content_id
{
    my $id = shift or die;
    return $id . 'Content';
}

sub _import_scripts
{
    [
        '/jqwidgets/jqxexpander.js'
    ];
}

sub _js_attrs
{
	my $self = shift;
	jsJoinAttrs(
		$self->SUPER::_js_attrs
        , jsAttrBoolean('expanded', $self->expanded)
	);
}

sub controller
{
    my $self = shift;
    
    my $id         = $self->id;
    my $idHeader   = _default_header_id($self->id);
    my $idContent  = _default_content_id($self->id);
    my $timeout    = $self->timeout;
    my $url        = $self->url;
    my $attrs      = $self->_js_attrs;
    
    my $r = <<"EndOfControllerData";
    \$("#$id").jqxExpander({ $attrs });
EndOfControllerData

    if (defined $url) {
        $r .= <<"EndOfControllerData";
    setTimeout(function () {
        \$.ajax({
            url: '$url',
            type: 'GET',
            dataType: 'json',
            success: function (data) {
                if(data.header)  \$("#$id").jqxExpander('setHeaderContent', data.header);
                if(data.content) \$("#$id").jqxExpander('setContent', data.content);
            }
        });
    }, $timeout);
EndOfControllerData
    }

    $r;
}

sub render
{
    my $self = shift;
    my $id = $self->id;
    my $idHeader  = _default_header_id($self->id);
    my $idContent = _default_content_id($self->id);
    
    my $html = qq[<div id='$id'>];
    $html .= qq[<div id='$idHeader'>];
    $html .= $self->_render_content($self->header);
    $html .= qq[</div>];
    $html .= qq[<div id='$idContent'>];
    $html .= $self->_render_content($self->content);
    $html .= qq[</div>];
	$html .= qq[</div>];

    $html;
}

1;

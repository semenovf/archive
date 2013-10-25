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

sub controller
{
    my $self = shift;
    
    my $id         = $self->id;
    my $idHeader   = _default_header_id($self->id);
    my $idContent  = _default_content_id($self->id);
    my $timeout    = $self->timeout;
    my $url        = $self->url;

    my $attrs = jsJoinAttrs(
          jsAttrString('theme', $self->global->theme)
        , jsAttrString('width', $self->width)
        , jsAttrString('height', $self->height)
        , jsAttrBoolean('expanded', $self->expanded)
    );
    
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

sub view
{
    my $self = shift;
    my $id = $self->id;
    my $idHeader  = _default_header_id($self->id);
    my $idContent = _default_content_id($self->id);
    
    my $headerText = 'Specification' // 'Loading Header...';
    
    my $html = <<"EndOfViewData";
<div id='$id'>
    <div id='$idHeader'>$headerText</div>
    <div id='$idContent'>Loading Content...</div>
</div>
EndOfViewData

    $html;
}

1;

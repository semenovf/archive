package HgWidget::Expander;
use HgWidget::Config;

use Mo qw(is default chain required);
extends 'HgWidget::Widget';

has id       => (is => 'rw', chain => 1, required => 1);
has header   => (is => 'rw', chain => 1, default => 'Rename the header...');
has content  => (is => 'rw', chain => 1, default => 'Set the content...');
has expanded => (is => 'rw', chain => 1, default => 1);
has url      => (is => 'rw', chain => 1, default => 'http://');
has timeout  => (is => 'rw', chain => 1, default => '500');

sub _to_boolean
{
    my $val = shift // 0;
    return $val
        ? ($val eq 'false' ? 'false' : 'true')
        : 'false';
}

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

sub controller
{
    my $self = shift;

    my $id        = $self->id;
    my $idHeader  = _default_header_id($self->id);
    my $idContent = _default_content_id($self->id);
    my $url       = $self->url;
    my $width     = $self->width;
    my $height    = $self->height;
    my $expanded  = _to_boolean($self->expanded);
    my $timeout   = $self->timeout;
    
    my $theme     = $self->theme // HgWidget::Config->get('theme');
    
    die "URL is mandatory" unless $url;
    
    my $html = <<"EndOfControllerData";
\$(document).ready(function () {
    \$("#$id").jqxExpander({ width:'$width', height:'$height', expanded:$expanded, theme:'$theme' });
    
    setTimeout(function () {
        \$.ajax({
            url: '$url',
            type: 'GET',
            dataType: 'json',
            success: function (data) {
                \$("#$id").jqxExpander('setHeaderContent', data.header);
                \$("#$id").jqxExpander('setContent', data.content);
            }
        });
    }, $timeout);
});
EndOfControllerData

    $html;
}

sub view
{
    my $self = shift;
    my $id = $self->id;
    my $idHeader  = _default_header_id($self->id);
    my $idContent = _default_content_id($self->id);
    
    my $html = <<"EndOfViewData";
    <div id='$id'>
    <div id='$idHeader'>
        Loading Header...</div>
    <div id='$idContent'>
        Loading Content...</div>
    </div>
EndOfViewData

    $html;
}

1;

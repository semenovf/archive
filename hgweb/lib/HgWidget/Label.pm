package HgWidget::Label;
use Mo qw(is default chain);
extends 'HgWidget::Widget';

has text => (is => 'rw', chain => 1, default => '');

sub render
{
    my $self = shift or die;
    my $id   = $self->id;
    my $text = $self->text;
    my $theme = $self->global->theme;
    my $styleClass = 'jqx-widget-content jqx-widget-content-' . $theme;
    
    my $html = <<"EndOfViewData";
<span id='$id' class="$styleClass">$text</span>
EndOfViewData

    $html;
}

1;

package HgWidget::Spacer;
use HgWidget::Util;
use Mo qw(is default chain required);
extends 'HgWidget::Widget';

has size    => (is => 'rw', chain => 1, default => undef);
has stretch => (is => 'rw', chain => 1, default => undef);

sub render
{
    my $self = shift;
   	my $width = 0;
   	
    if (defined $self->size) {
    	$width = $self->size . 'px';
    } elsif (defined $self->stretch) {
    	$width = $self->stretch >= 100 
    		? '100%'
    		: $self->stretch <= 0
    			? 0
    			: $self->stretch . '%';
    }
   
    qq[<p style="width: $width; display: inline-block; margin: 0; padding: 0; border: 0">&nbsp;</p>];
}

1;

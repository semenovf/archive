package HgWidget::Layout;
use Mo qw(is default chain required);

has elements => (is=>'ro', default=>[]);

=item $layout->add($widget, ...)
=item $layout->add($layout, ...)
=cut

sub add
{
	my $self = shift;
	push @{$self->{elements}}, @_;
	$self;
}

sub render
{
	my $self = shift;
	my $r = '';
	for my $w (@{$self->{elements}}) {
		$r .= $w->render;
	}
	
	$r;
}

1;
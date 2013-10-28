package HgWidget::GridLayout;
use Mo qw(is default chain required);
extends 'HgWidget::Layout';

=item $layout->add($widget, $row, $col)
=item $layout->add($layout, $row, $col)
=cut

sub add
{
	my $self = shift or die;
	my $w    = shift or die;
	my $row  = shift;
	my $col  = shift;

	die unless defined $row;
	die unless defined $col;

	$self->{elements}->[$row] = () unless defined $self->{elements}->[$row];
	$self->{elements}->[$row][$col] = $w;
}

sub render
{
	my $self = shift;
	my $nrows = scalar(@{$self->{elements}});
	
	my $r = '<table cellpadding="0" cellspacing="0" style="border-collapse: collapse; border: 0 none #000;">';
	for (my $row = 0; $row < $nrows; ++$row) {
		$r .= '<tr>';
		my $ncols = scalar(@{$self->{elements}->[$row]});
		for (my $col = 0; $col < $ncols; ++$col) {
			if (defined $self->{elements}->[$row][$col]) {
				$r .= '<td>' . $self->{elements}->[$row][$col]->render . '</td>';
			} else {
				$r .= '<td>&nbsp;</td>';
			}
		}
		$r .= '</tr>';
	}
	
	$r .= '</table>';
	$r;
}


1;
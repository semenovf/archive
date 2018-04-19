package HgWidget::Global;
use Mo qw(is default chain);

has theme  => (is => 'rw', chain => 1, default => 'arctic');

1;
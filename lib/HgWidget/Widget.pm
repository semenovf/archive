package HgWidget::Widget;
use HgWidget::Config;
use Mo qw(is default chain);

has theme   => (is => 'rw', chain => 1, default => '');
has width   => (is => 'rw', chain => 1, default => '100%');
has height  => (is => 'rw', chain => 1, default => '100%');

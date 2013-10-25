package HgWidget::Util;
use Exporter 'import';
our @EXPORT = qw(booleanString jsJoinAttrs jsAttrBoolean jsAttrString jsAttrVar jsAttrNumber);

sub booleanString
{
    my $val = shift;
    return $val
        ? ($val eq 'false' ? 'false' : 'true')
        : 'false';
}

sub jsJoinAttrs
{
    my @attrs = @_;
    my $r  = '';
    
    foreach my $a (@attrs) {
        next unless defined $a;
        $r .= ' ,' if length($r) > 0;
        $r .= $a;
    }
    $r;
}

sub jsAttrBoolean
{
    my ($name, $val, $default) = @_;
    $val = $default unless defined $val;
    defined $val ? "$name: " . booleanString($val) : undef;
}

sub jsAttrString
{
    my ($name, $val, $default) = @_;
    $val = $default unless defined $val;
    defined $val ? "$name: '$val'" : undef;
}

sub jsAttrVar
{
    my ($name, $val, $default) = @_;
    $val = $default unless defined $val;
    defined $val ? "$name: $val" : undef;
}

sub jsAttrNumber
{
    return jsAttrVar(@_);
}
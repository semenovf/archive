package DBI::DDI::SQL;

sub sql_stringify_bool_type { "BOOLEAN" }

sub sql_stringify_int_type
{
    my ($min, $max) = @_;
    
    $max += 0;
    $min += 0;

    my $type = '';

    if ($max <= 0xFF) {
        $type = "TINYINT";
    } elsif ($max <= 0xFFFF) {
        $type = "SMALLINT";
    } elsif ($max <= 0xFFFFFF) {
        $type = "MEDIUMINT";
    } elsif ($max <= 0xFFFFFFFF) {
        $type = "INT";
    } else {
        $type = "BIGINT";
    }
    
    if ($min >= 0) { # unsigned
        $type .= ' UNSIGNED';
    } 
    return $type;
}

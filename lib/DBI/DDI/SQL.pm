package DBI::DDI::SQL;

sub sql_stringify_bool_type { "BOOLEAN" }

sub sql_stringify_int_type
{
    my ($min, $max) = @_;

    if ($max > 0xCWT_LONGLONG_MAX && ! ($min >= 0) ) {
        die 'Illegal boubde for Interger type' ;
    }

    if( $min >= 0 ) { /* unsigned */
if( max <= (ULONGLONG)CWT_UINT8_MAX )
s = strNS->strDup(_T("TINYINT UNSIGNED"));
else if( max <= (ULONGLONG)CWT_UINT16_MAX )
s = strNS->strDup(_T("SMALLINT UNSIGNED"));
else if( max <= (ULONGLONG)CWT_UINT24_MAX )
s = strNS->strDup(_T("MEDIUMINT UNSIGNED"));
else if( max <= (ULONGLONG)CWT_UINT32_MAX )
s = strNS->strDup(_T("INT UNSIGNED"));
else
s = strNS->strDup(_T("BIGINT UNSIGNED"));
} else {
if( min >= (LONGLONG)CWT_INT8_MIN && max <= (ULONGLONG)CWT_INT8_MAX )
s = strNS->strDup(_T("TINYINT"));
else if( min >= (LONGLONG)CWT_INT16_MIN && max <= (ULONGLONG)CWT_INT16_MAX )
s = strNS->strDup(_T("SMALLINT"));
else if( min >= (LONGLONG)CWT_INT24_MIN && max <= (ULONGLONG)CWT_INT24_MAX )
s = strNS->strDup(_T("MEDIUMINT"));
else if( min >= (LONGLONG)CWT_INT32_MIN && max <= (ULONGLONG)CWT_INT32_MAX )
s = strNS->strDup(_T("INT"));
else
s = strNS->strDup(_T("BIGINT"));
}

return s;
}


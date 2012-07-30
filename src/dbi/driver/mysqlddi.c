/*
 * mysqlddi.c
 *
 *  Created on: 30.07.2012
 *      Author: user
 */


#include <cwt/dbi/dbi.h>
#include <cwt/dbi/ddi.h>
#include <cwt/strlist.h>


static const CWT_CHAR *__MYSQL_DEFAULT_CHARSET   = _T("utf8");
static const CWT_CHAR *__MYSQL_DEFAULT_DB_ENGINE = _T("InnoDB");


void __specForDeploy(CwtStrList *ddiSql, const CWT_CHAR *ns, const CwtDDI *ddi, const CWT_CHAR *charset, UINT ddiflags)
{
	CwtStrNS     *strNS    = cwtStrNS();
	CwtStringNS  *stringNS = cwtStringNS();
	CwtStrListNS *slNS     = cwtStrListNS();

	CwtString  *sbuf = stringNS->create();
    CwtStrList *ddiForeignKeys; /* @ddiForeignKeys = ();*/
    CwtStrList *ddiConstraints; /* @ddiConstraints = ();*/
    CwtStrList *ddiIndices;     /* @ddiIndices = ();*/
/*    my %ddiConstraintsColumns = (); # use for generate unique constraints names*/
    CWT_CHAR *dbEngine = __MYSQL_DEFAULT_DB_ENGINE;

    CWT_ASSERT(ns);
    CWT_ASSERT(ddi);

    if( !charset )
    	charset = __MYSQL_DEFAULT_CHARSET;

    sbuf           = stringNS->create();
    ddiForeignKeys = slNS->create();
    ddiConstraints = slNS->create();
    ddiIndices     = slNS->create();

    if( ddiflags & CWT_DDI_DO_NOT_CREATE_DB ) {
    	stringNS->sprintf(sbuf, _T("CREATE DATABASE IF NOT EXISTS `%s`"), ns);
    	slNS->add(ddiSql, stringNS->cstr(sbuf));
    }

    stringNS->sprintf(sbuf, _T("USE `%s`"), ns);
    slNS->add(ddiSql, stringNS->cstr(sbuf));

    for(;;) { /*foreach my $table ( keys %{$ddi} ) {*/

    	CwtStrList *columnDefs; /* @columnDefs = () */
        my $pk = '';
        BOOL hasAutoinc = FALSE;

        columnDefs = slNS->create();

        for(;;) { /*foreach my $colName ( sort keys %{$ddi->{$table}} ) {*/
            my $colDef = $ddi->{$table}->{$colName};
            CwtDDI_Type colType = $colDef->{-type};

            my $notnull = '';
            my $autoinc = '';

            if( defined $colDef->{-notnull} && $colDef->{-notnull} ) {
                $notnull = ' NOT NULL';
            }

            if( defined $colDef->{-pk} && $colDef->{-pk} ) {
                croak "Only one PRIMARY KEY is supported" if $pk;
                $pk = sprintf(', PRIMARY KEY (`%s`)', $colName);
            }

            if( defined $colDef->{-autoinc} && $colDef->{-autoinc} ) {
                croak "Only for one column AUTO_INCREMENT is supported" if $hasAutoinc;
                $autoinc = ' AUTO_INCREMENT';
                hasAutoinc = TRUE;
            }

            switch(colType) {
            case CWT_DDI_TYPE_BOOL:
				stringNS->sprintf(sbuf, _T(" `%s` BOOLEAN NOT NULL"), colName);
				slNS->add(columnDefs, stringNS->cstr(sbuf));
                break;

            case CWT_DDI_TYPE_INT
                    my $min = $colDef->{-min};
                    my $max = $colDef->{-max};
                    push @columnDefs, sprintf(' `%s` %s%s%s' , $colName, map_ddi_int($min, $max), $notnull, $autoinc);
                    break;

            case CWT_DDI_TYPE_TIME:
                    push @columnDefs, sprintf(' `%s` TIME NOT NULL', $colName);
                    break;

            case CWT_DDI_TYPE_DATE:
                    push @columnDefs, sprintf(' `%s` DATE NOT NULL', $colName);
                    break;

            case CWT_DDI_TYPE_DATETIME:
                    push @columnDefs, sprintf(' `%s` DATETIME NOT NULL', $colName);
                    break;

            case CWT_DDI_TYPE_TIMESTAMP:
                    push @columnDefs, sprintf(' `%s` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP', $colName);
                    break;

            case CWT_DDI_TYPE_FLOAT:
                	break;

            case CWT_DDI_TYPE_TEXT:
                    my $len = $colDef->{-length};
                    push @columnDefs, sprintf(' `%s` %s%s%s', $colName, map_ddi_text($len), $notnull, $autoinc);
                    break;

            case CWT_DDI_TYPE_REF:
                    my $refTab = $colDef->{-ref};

                    die sprintf('Referenced class \'%s\' not found in namespace \'%s\'', $refTab, $ns) unless defined $ddi->{$refTab};

                    my $refPk = find_ref_pk( $ddi->{$refTab} );

                    croak sprintf('PK not found in referenced class \'%s\' of namespace \'%s\'', $refTab, $ns) unless $refPk;
                    my $min = $ddi->{$refTab}->{$refPk}->{-min};
                    my $max = $ddi->{$refTab}->{$refPk}->{-max};
                    push @columnDefs, sprintf(' `%s` %s' , $colName, map_ddi_int($min, $max));
                    push @ddiForeignKeys, sprintf('ALTER TABLE %s ADD KEY `FK_%s` (`%s`)', $table, $colName, $colName);

                    /* http://dev.mysql.com/doc/refman/5.0/en/innodb-foreign-key-constraints.html
                      if the CONSTRAINT symbol clause is given, the symbol value must be unique
                      in the database. If the clause is not given, InnoDB creates the name automatically. */
                    unless ( exists $ddiConstraintsColumns{ $colName } ) {
                        $ddiConstraintsColumns{$colName} = 0;
                    } else {
                        $ddiConstraintsColumns{$colName}++;
                    }
                    push @ddiConstraints, sprintf('ALTER TABLE `%s` ADD CONSTRAINT `FK_%s_%d` FOREIGN KEY (`%s`) REFERENCES `%s` (`%s`)',
                        $table, $colName, $ddiConstraintsColumns{$colName}, $colName, $refTab, $refPk );
                    break;

            case CWT_DDI_TYPE_INDEX_UNIQUE:
                    push @ddiIndices, sprintf('ALTER TABLE `%s` ADD UNIQUE INDEX `%s` (%s)',
                        $table, $colName, join(',', map { '`'.$_.'`'} @{$colDef->{-columns}} ) );
                    break;

            default:
                croak sprintf('Invalid data type for column in table `%s`', $table) ;
                break;
            } /* switch(colType) */
        }

        stringNS->sprintf(sbuf, _T("CREATE TABLE %s `%s` (%s %s) ENGINE=%s AUTO_INCREMENT=5 DEFAULT CHARSET=%s")
        	, ( ddiflags & CWT_DDI_CREATE_TAB_IF_NOT_EXISTS ) ? "IF NOT EXISTS" : ""
        	, $table
        	, join( ',', @columnDefs)
        	, $pk
        	, $dbEngine
        	, $charset);
        slNS->add(ddiSql, stringNS->cstr(sbuf));

        slNS->free(columnDefs);
    }

    push @ddiSQL, @ddiForeignKeys if @ddiForeignKeys > 0;
    push @ddiSQL, @ddiConstraints if @ddiConstraints > 0;
    push @ddiSQL, @ddiIndices if @ddiIndices > 0;

    slNS->free(ddiForeignKeys);
    slNS->free(ddiConstraints);
    slNS->free(ddiIndices);
    stringNS->free(sbuf);
}

void __specForRecall(CwtStrList *sql)
{
    my ($self, %args) = @_;
    my $ns = $args{-NS} or croak 'namespace expected';
    my $ddi = $args{-DDI} or croak 'ddi expected';
    my $flags = $args{-Flags};

    my @ddiSQL = ();
    my @ddiSQLDropFK = ();
    my @ddiSQLDropTables = ();
    my %ddiConstraintsColumns = (); # use for generate unique constraints names

    push @ddiSQL, sprintf('USE `%s`',$ns);

    foreach my $table ( keys %{$ddi} ) {
        foreach my $colName ( sort keys %{ $ddi->{$table} } ) {

            my $colType = $ddi->{$table}->{$colName}->{-type};

            SWITCH: {
                $colType == DDI_TYPE_REF && do {
                    unless ( exists $ddiConstraintsColumns{ $colName } ) {
                        $ddiConstraintsColumns{$colName} = 0;
                    } else {
                        $ddiConstraintsColumns{$colName}++;
                    }
                    push @ddiSQLDropFK, sprintf('ALTER TABLE %s DROP FOREIGN KEY `FK_%s_%d`',
                        $table, $colName, $ddiConstraintsColumns{$colName} );
                    last;
                };
            }
        }
        push @ddiSQLDropTables, sprintf('DROP TABLE IF EXISTS `%s`',$table);
    }

    push @ddiSQL, @ddiSQLDropFK;
    push @ddiSQL, @ddiSQLDropTables;
    return wantarray ? @ddiSQL : join(";\n", @ddiSQL);
}


sub find_ref_pk
{
    my $tabSpec = shift;

    foreach my $colName ( keys ( %{$tabSpec} ) ) {
        return $colName if defined $tabSpec->{$colName}->{-pk} && $tabSpec->{$colName}->{-pk};
    }

    return 0;
}


static map_ddi_int(ssize_t min, ssize_t max)
{
    my $spec;
    BOOL isUnsigned = FALSE;

    if( ref $min && ref $min eq 'CODE' ) {
        die "Invalid value for minimum for integer" unless( &$min eq &{&DDI_MIN_INT} );
        $min = MYSQL_MIN_BIGINT;
    }

    if( ref $max && ref $max eq 'CODE' ) {
        die "Invalid value for maximum for integer" unless( &$max eq &{&DDI_MAX_INT} );
        $max = MYSQL_MAX_BIGINT;
    }

    die "Maximum value is less or equal than minimum for integer" if( $min >= $max );

    if( min >= 0 )
    	isUnsigned = TRUE;

    if( ($isUnsigned && $max <= MYSQL_MAX_TINYINT_UNSIGNED) ||
        ($min >= MYSQL_MIN_TINYINT && $max <= MYSQL_MAX_TINYINT) ) {
        $spec = 'TINYINT';
    } elsif ( ($isUnsigned && $max <= MYSQL_MAX_SMALLINT_UNSIGNED) ||
        ($min >= MYSQL_MIN_SMALLINT && $max <= MYSQL_MAX_SMALLINT) ) {
        $spec = 'SMALLINT';
    } elsif ( ($isUnsigned && $max <= MYSQL_MAX_MEDIUMINT_UNSIGNED) ||
        ($min >= MYSQL_MIN_MEDIUMINT && $max <= MYSQL_MAX_MEDIUMINT) ) {
        $spec = 'MEDIUMINT';
    } elsif ( ($isUnsigned && $max <= MYSQL_MAX_INT_UNSIGNED) ||
        ($min >= MYSQL_MIN_INT && $max <= MYSQL_MAX_INT) ) {
        $spec = 'INTEGER';
    } elsif ( ($isUnsigned && $max <= MYSQL_MAX_BIGINT_UNSIGNED) ||
        ($min >= MYSQL_MIN_BIGINT && $max <= MYSQL_MAX_BIGINT) ) {
        $spec = 'BIGINT';
    } else {
        warn "Expected valid range for integer. Cast to BIGINT";
        $spec = 'BIGINT';
    }

    $spec .= ' UNSIGNED' if $isUnsigned;
    $spec .= ' NOT NULL';
    return $spec;
}


sub map_ddi_text
{
    my $len = shift;
    my $spec;

    if( ref $len eq 'CODE' ) {
        croak "Invalid length for text" unless &$len eq &{&DDI_TEXT_MAXLEN};
        $spec = 'TEXT';
    } else {
        croak "Invalid length for text" if $len <= 0;
        $spec = sprintf('VARCHAR(%d)', $len);
    }
    return $spec;
}

package DBI::DDI::MySQL;
use Carp;
use DBI::DDI::Const;

use strict;
use warnings;

sub MYSQL_DEFAULT_CHARSET        { 'utf8' }
sub MYSQL_DEFAULT_DB_ENGINE      { 'InnoDB' }

sub MYSQL_MAX_TINYINT_UNSIGNED   {                  255 }
sub MYSQL_MAX_TINYINT            {                  127 }
sub MYSQL_MIN_TINYINT            {                 -128 }
sub MYSQL_MAX_SMALLINT_UNSIGNED  {                65535 }
sub MYSQL_MAX_SMALLINT           {                32767 }
sub MYSQL_MIN_SMALLINT           {               -32768 }
sub MYSQL_MAX_MEDIUMINT_UNSIGNED {             16777215 }
sub MYSQL_MAX_MEDIUMINT          {              8388607 }
sub MYSQL_MIN_MEDIUMINT          {             -8388608 }
sub MYSQL_MAX_INT_UNSIGNED       {           4294967295 }
sub MYSQL_MAX_INT                {           2147483647 }
sub MYSQL_MIN_INT                {          -2147483648 }
sub MYSQL_MAX_BIGINT_UNSIGNED    { 18446744073709551615 }
sub MYSQL_MAX_BIGINT             {  9223372036854775807 }
sub MYSQL_MIN_BIGINT             { -9223372036854775808 }

sub spec_for_deploy
{
    my ($self, %args) = @_;
    my $ns = $args{-NS} or croak 'namespace expected';
    my $ddi = $args{-DDI} or croak 'ddi exepcted';
    my $charset = $args{-Charset} || MYSQL_DEFAULT_CHARSET;

    my @ddiSQL = ();
    my @ddiForeignKeys = ();
    my @ddiConstraints = ();
    my @ddiIndices = ();
    my %ddiConstraintsColumns = (); # use for generate unique constraints names

    my $dbEngine   = MYSQL_DEFAULT_DB_ENGINE;

    push @ddiSQL, sprintf('CREATE DATABASE IF NOT EXISTS `%s`', $ns);
    push @ddiSQL, sprintf('USE `%s`',$ns);

    foreach my $table ( keys %{$ddi} ) {

        my @columnDefs = ();
        my $pk = '';
        my $hasAutoinc = 0;

        foreach my $colName ( sort keys %{$ddi->{$table}} ) {
            my $colDef = $ddi->{$table}->{$colName};
            my $colType = $colDef->{-type};

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
                $hasAutoinc = 1;
            }

            SWITCH: {
                $colType == DDI_TYPE_BOOL && do {
                    push @columnDefs, sprintf(' `%s` BOOLEAN NOT NULL', $colName);
                    last SWITCH;
                };

                $colType == DDI_TYPE_INT && do {
                    my $min = $colDef->{-min};
                    my $max = $colDef->{-max};
                    push @columnDefs, sprintf(' `%s` %s%s%s' , $colName, map_ddi_int($min, $max), $notnull, $autoinc);
                    last SWITCH;
                };

                $colType == DDI_TYPE_TIME && do {
                    push @columnDefs, sprintf(' `%s` TIME NOT NULL', $colName);
                    last SWITCH;
                };

                $colType == DDI_TYPE_DATE && do {
                    push @columnDefs, sprintf(' `%s` DATE NOT NULL', $colName);
                    last SWITCH;
                };

                $colType == DDI_TYPE_DATETIME && do {
                    push @columnDefs, sprintf(' `%s` DATETIME NOT NULL', $colName);
                    last SWITCH;
                };

                $colType == DDI_TYPE_TIMESTAMP && do {
                    push @columnDefs, sprintf(' `%s` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP', $colName);
                    last SWITCH;
                };

                $colType == DDI_TYPE_FLOAT && do {
                    last SWITCH;
                };

                $colType == DDI_TYPE_TEXT && do {
                    my $len = $colDef->{-length};
                    push @columnDefs, sprintf(' `%s` %s%s%s', $colName, map_ddi_text($len), $notnull, $autoinc);
                    last SWITCH;
                };

                $colType == DDI_TYPE_REF && do {
                    my $refTab = $colDef->{-ref};

                    die sprintf('Referenced class \'%s\' not found in namespace \'%s\'', $refTab, $ns) unless defined $ddi->{$refTab};

                    my $refPk  = find_ref_pk( $ddi->{$refTab} );

                    croak sprintf('PK not found in referenced class \'%s\' of namespace \'%s\'', $refTab, $ns) unless $refPk;
                    my $min = $ddi->{$refTab}->{$refPk}->{-min};
                    my $max = $ddi->{$refTab}->{$refPk}->{-max};
                    push @columnDefs, sprintf(' `%s` %s' , $colName, map_ddi_int($min, $max));
                    push @ddiForeignKeys, sprintf('ALTER TABLE %s ADD KEY `FK_%s` (`%s`)', $table, $colName, $colName);

                    # http://dev.mysql.com/doc/refman/5.0/en/innodb-foreign-key-constraints.html
                    # if the CONSTRAINT symbol clause is given, the symbol value must be unique
                    # in the database. If the clause is not given, InnoDB creates the name automatically.
                    unless ( exists $ddiConstraintsColumns{ $colName } ) {
                        $ddiConstraintsColumns{$colName} = 0;
                    } else {
                        $ddiConstraintsColumns{$colName}++;
                    }
                    push @ddiConstraints, sprintf('ALTER TABLE `%s` ADD CONSTRAINT `FK_%s_%d` FOREIGN KEY (`%s`) REFERENCES `%s` (`%s`)',
                        $table, $colName, $ddiConstraintsColumns{$colName}, $colName, $refTab, $refPk );
                    last SWITCH;
                };

                $colType == DDI_TYPE_INDEX_UNIQUE && do {
                    push @ddiIndices, sprintf('ALTER TABLE `%s` ADD UNIQUE INDEX `%s` (%s)',
                        $table, $colName, join(',', map { '`'.$_.'`'} @{$colDef->{-columns}} )  );
                    last SWITCH;
                };

                croak sprintf('Invalid data type for column in table `%s`', $table) ;
            }
        }


        push @ddiSQL, sprintf('CREATE TABLE `%s` (%s %s) ENGINE=%s AUTO_INCREMENT=5 DEFAULT CHARSET=%s',
            $table,
            join( ',', @columnDefs),
            $pk,
            $dbEngine, $charset );
    }

    push @ddiSQL, @ddiForeignKeys if @ddiForeignKeys > 0;
    push @ddiSQL, @ddiConstraints if @ddiConstraints > 0;
    push @ddiSQL, @ddiIndices if @ddiIndices > 0;

    return wantarray ? @ddiSQL : join(";\n", @ddiSQL);
}



sub spec_for_recall
{
    my ($self, %args) = @_;
    my $ns = $args{-NS} or croak 'namespace expected';
    my $ddi = $args{-DDI} or croak 'ddi exepcted';

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


sub map_ddi_int
{
    my ($min, $max) = @_;

    my $spec;
    my $isUnsigned = 0;

    if( ref $min && ref $min eq 'CODE' ) {
        die "Invalid value for minimum for integer" unless( &$min eq &{&DDI_MIN_INT} );
        $min = MYSQL_MIN_BIGINT;
    }

    if( ref $max && ref $max eq 'CODE' ) {
        die "Invalid value for maximum for integer" unless( &$max eq &{&DDI_MAX_INT} );
        $max = MYSQL_MAX_BIGINT;
    }

    die "Maximum value is less or equal than minimum for integer" if( $min >= $max );

    $isUnsigned = 1 if $min >= 0;

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

1;

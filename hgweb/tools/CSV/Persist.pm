package CSV::Persist;
use DBI;
use JSON;
use Text::CSV::Encoded;

use strict;
use warnings;

sub switch_param {
    my ($self, $prefix, $pname) = @_;
    my $prefixed = join '.', $prefix, $pname;
    return $self->{$prefixed} if defined $self->{$prefixed};
    return $self->{$pname} if defined $self->{$pname};
    return undef;
}

#
# CSV::Persist->new(Config=>json_file_path [, Prefix=>prefix])
#
sub new {
    my $class = shift or die;
    $class = ref $class if ref $class;
    
    my %args = @_;
    die q('Config' argument expected) unless $args{Config};
    my $pfx = $args{Prefix} || '';
    
    my $self;
    local $/;
    open( my $fh, '<', $args{Config} );
    $self = decode_json(<$fh>);
    close($fh);
    
    $self->{db_user}     = switch_param($self, $pfx, 'db_user');
    $self->{db_password} = switch_param($self, $pfx, 'db_password');
    $self->{db_host}     = switch_param($self, $pfx, 'db_host') || 'localhost';
    $self->{db_port}     = switch_param($self, $pfx, 'db_port') || 3306;
    $self->{table}       = switch_param($self, $pfx, 'table') || die q(Table name must be specfied by 'table' parameter);
    $self->{scheme}      = undef; # will be intialized later in load_scheme
    
    return bless $self, $class;
}


=head
> cat file.csv.json
{
      "sep_char"     : ','
    , "quote_char"   : '"'
    , "not_embed_nl" : 1
    , "encoding_in"  : "utf8" 
    , "encoding_out" : "utf8"
    , "header"       : true // header exists (first non-comment line )
    , "comment_str"  : "#"  // ignore lines begin with 'comment_str' string
    , "fields"       : [
            { "name" :"code", "type" : "CHAR(2)", "spec" : "PK" }  // spec: "PK;Nullable;Autoinc"
          , { "name" :"name", "type" : "TEXT(45)", "spec" : "" }
          , { "name" :"currency", "type" : "CHAR(3)", "spec" : "" }
    ]
}

Data types:
    INT(min,max)
    FLOAT(prec,scale)
    DOUBLE(prec,scale)
    TEXT(maxlen)
    BLOB(maxlen)
    TIME(is_stamp)
    DATE(is_stamp)
    DATETIME(is_stamp)
    # REF(entity) // Not used
    BOOL

Flags
    # GN // Not used
	UQ
	PK
	IX  // Indexable
	NA  // Nullable
	AI  // Autoincremented
	IN  // Insertable, not used yet
	UP  // Updatable, not used yet

=cut

sub load_scheme
{
    my ($self, $csv_file) = @_;
    my $json_file = $csv_file . '.json';
    
    die $json_file . ': CSV persistence configuration file not found' unless -f $json_file;
    local $/;
    open( my $fh, '<', $json_file );
    my $pconfig = decode_json(<$fh>);
    close($fh);
    
    unless ($pconfig->{not_embed_nl}) {
        $pconfig->{binary} = 1;
        $pconfig->{eol}    = $/; # for supporting embedded newlines
    }
    $self->{scheme} = $pconfig;
    $self->{scheme}->{skip_header} = 0 unless defined $self->{scheme}->{skip_header};
    return $pconfig;
}

sub sql_create_table_spec
{
    my ($self) = @_;
    my $pk;
    
    my $scolon = '';
    my $spec = 'CREATE TABLE IF NOT EXISTS ' . $self->{table};
    $spec .= '(';
    foreach my $f (@$self->{scheme}->fields) {
        my %fspec = map {  lc($_) => 1  } split /;/, $f->{spec};
        $spec .= $scolon;
        $spec .= '`'. $f->{name} . '`';
        $spec .= ' ' . $f->{type};
        $spec .= ' NOT NULL' unless defined $fspec{nullable};
        $spec .= ' AUTO_INCREMENT' unless defined $fspec{autoinc};
        $pk = $f->{name} if defined $fspec{pk};
        $scolon = ', ';
    }
    $spec .= ', PRIMARY KEY (`' . $pk . '`)' if defined $pk;
    $spec .= ') ENGINE=InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci';
    return $spec;    
}


sub sql_insert_spec
{
    my ($self) = @_;
    my $scolon = '';

    my $spec = 'INSERT INTO ' . $self->{table};
    my $values = '(';

    $spec .= '(';
    foreach my $f (@$self->{scheme}->fields) {
        my %fspec = map {  lc($_) => 1  } split /;/, $f->{spec};
        $spec .= $scolon;
        $values .= $scolon . '?';
        $spec .= '`'. $f->{name} . '`';
        $scolon = ', ';
    }
    $values .= ')';
    $spec .= ') VALUES ' . $values;
    return $spec;    
}


sub persist
{
    my ($self, $csv_file) = @_;
    
    open my $in, "<", $csv_file or die "$csv_file: $!";
    my $dbh = DBI->connect('dbi:mysql:database='
            . $self->{db_name}
            . ';host=' . $self->{db_host}
            . ';port=' . $self->{db_port}
            , $self->{db_user}
            , $self->{db_password}
            , { RaiseError => 0, AutoCommit => 0 }) or die 'unable to disconnect to database';
    
    print 'Connected to database by ', $self->{db_user}, '@', $self->{db_host}, ':', $self->{db_port}, "\n";
    
    my $csv = Text::CSV::Encoded->new ({
        $self->load_scheme($csv_file)
    });
    
    my $line = 0;
    
    if ($self->{scheme}->{skip_header}) {
        $csv->getline ($in);
        ++$line;
    }
    
    $dbh->do('DROP TABLE IF EXISTS ' . $self->{table}) or die $dbh->errstr;
    $dbh->do($self->sql_create_table_spec) or die $dbh->errstr;
    my $sth = $dbh->prepare($self->sql_insert_spec) or die $dbh->errstr;

    while (my $row = $csv->getline ($in)) {
        ++$line;
        my @fields = @$row;
        next unless @fields;
        next if $fields[0] =~ /^\s*#/; # skip comment lines

        my $data = {code => undef, name => undef, currency => undef};
        SWITCH: {
            (@fields == 7) and do { $data->{code} = $fields[1]; $data->{name} = $fields[2]; $data->{currency} = $fields[5]; last; };
            (@fields == 6) and do { $data->{code} = $fields[0]; $data->{name} = $fields[1]; $data->{currency} = $fields[4]; last; };
            (@fields == 4) and do { $data->{code} = $fields[0]; $data->{name} = $fields[1]; $data->{currency} = $fields[2]; last; };
            (@fields == 3) and do { $data->{code} = $fields[0]; $data->{name} = $fields[1]; $data->{currency} = $fields[2]; last; };
            die 'invalid row format at line ' , $line, "\n";
        };
    
#        die 'invalid country code at line ', $line, "\n" unless length($data->{code}) == 2;
#        die 'invalid currency code at line ', $line, "\n" unless length($data->{currency}) == 3 || length($data->{currency}) == 0; # Antarctica has no currency
        
        #print join(' ', $data->{code},  $data->{name}, $data->{currency}), "\n";
        $sth->execute($data->{code},  $data->{name}, $data->{currency}) or die $dbh->errstr;
        
        undef $data;
    }
    close($in);
    $dbh->commit;
    $dbh->disconnect;
}

1;

package CSV::Persist;
use DBI;
use JSON;

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
    
    return bless $self, $class;
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
          sep_char => ','
        , quote_char => '"'
        , binary => 1, eol => $/ # for supporting embedded newlines
        , encoding_in  => "utf8", 
        , encoding_out => "utf8", 
    });
    
    my $line = 0;
    
    # Find header.
    # Header format:
    # # %header
    # # id,code$CHAR(2)$NN,name$VARCHAR(45)$NN,latitude,longitude,currency$CHAR(3)$NN,timezone
    my @header;
    my $header_expect = 0;
    while (my $row = $csv->getline ($in)) {
        ++$line;
        @header = @$row;
        next unless @header;
        #next if $fields[0] =~ /^#/; # ommit comment lines (first column begins with '#')
        if ($header_expect) {
            if ($header[0] =~ /^#/) {
                # must be an actual header
                last;
            } else {
                die "Header expected\n";
            }
        } else {
            if ($header[0] =~ /^#\s*%header/) {
                $header_expect = 1;
            } elsif($header[0] =~ /^#/) {
                ;
            } else {
                die "Header not found\n";
            }
        }
    }

    $header[0] =~ /^#\s*(.*)/ && do { $header[0] = $1; };
    print join('-', @header), "\n";

    $dbh->do('DROP TABLE IF EXISTS ' . $self->{table}) or die $dbh->errstr;
    
    $dbh->do('CREATE TABLE IF NOT EXISTS '
        . $self->{table}
        . '(`code` CHAR(2)  NOT NULL,
            `name` VARCHAR(45) NOT NULL,
            `currency` CHAR(3) NOT NULL,
            PRIMARY KEY (`code`)
            ) ENGINE=InnoDB DEFAULT CHARACTER SET = utf8 COLLATE = utf8_general_ci' ) or die $dbh->errstr;

    my $sth = $dbh->prepare('INSERT INTO '
        .  $self->{table}
        . ' (code, name, currency) VALUES (?, ?, ?)') or die $dbh->errstr;


    
    while (my $row = $csv->getline ($in)) {
        ++$line;
        my @fields = @$row;
        next unless @fields;
        next if $fields[0] =~ /^#/; # ommit comment lines (first column begins with '#')
        my $data = {code => undef, name => undef, currency => undef};
        SWITCH: {
            (@fields == 7) and do { $data->{code} = $fields[1]; $data->{name} = $fields[2]; $data->{currency} = $fields[5]; last; };
            (@fields == 6) and do { $data->{code} = $fields[0]; $data->{name} = $fields[1]; $data->{currency} = $fields[4]; last; };
            (@fields == 4) and do { $data->{code} = $fields[0]; $data->{name} = $fields[1]; $data->{currency} = $fields[2]; last; };
            (@fields == 3) and do { $data->{code} = $fields[0]; $data->{name} = $fields[1]; $data->{currency} = $fields[2]; last; };
            die 'invalid row format at line ' , $line, "\n";
        };
    
        die 'invalid country code at line ', $line, "\n" unless length($data->{code}) == 2;
        die 'invalid currency code at line ', $line, "\n" unless length($data->{currency}) == 3 || length($data->{currency}) == 0; # Antarctica has no currency
        
        #print join(' ', $data->{code},  $data->{name}, $data->{currency}), "\n";
        $sth->execute($data->{code},  $data->{name}, $data->{currency}) or die $dbh->errstr;
        
        undef $data;
    }
    close($in);
    $dbh->commit;
    $dbh->disconnect;
}

1;

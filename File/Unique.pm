package JQ::File::Unique;
use File::Spec::Functions qw(catfile);


require Exporter;
our @ISA = qw(Exporter);
our @EXPORT = qw(uniqueFileName);

# Генерирует уникальное имя файла в указанном каталоге
sub uniqueFileName
{
    my $dir = shift;
    my $prefix = shift // "";
    my $suffix = shift // "";
    my $tries  = shift // 5;
  
    my $path;
  
    while( $tries-- ) {
        my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime(time);
        my $fn = join( '',
            $prefix,
            sprintf("%d%02d%02d%02d%02d%02d", $year+1900, $mon+1, $mday, $hour, $min, $sec),
            '_',
            join( '', ('0'..'9', 'A'..'Z', 'a'..'z')[rand 62, rand 62, rand 62, rand 62, rand 62, rand 62, rand 62, rand 62]),
            $suffix );
    
        $path = catfile( $dir, $fn );
        last unless -e $path;
    
        warn("Generating unique filepath ($path) failed, will make next attempt...");
        $path = undef;
    }

        
    unless( $path ) {
        die "Generating unique filepath failed";
    }
    
    return $path;
}

1;
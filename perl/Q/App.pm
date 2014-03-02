package JQ::App;
use Data::Dumper;
use Getopt::Long;
use Pod::Usage;
use JQ::Logger;
use base qw(Class::Accessor);
use strict;
use warnings;

my $_theApp = undef;
my @_ARGV;

__PACKAGE__->mk_accessors(qw(
    _prefs
    _timer
    _argv
));

sub instance
{
    my $self = shift;
    die "Application instance must be initialized" unless defined $_theApp;
    return $_theApp;
}

sub new
{
    my ($class, %args) = @_;
    my $isHelp  = 0;
    my $isMan   = 0;
    my $startTime = time();

    return $_theApp if defined $_theApp;

    @_ARGV = @ARGV;
    unless ( defined $args{-Prefs} ) {
        die "You must specify -Prefs for the application\n";
    }

    if( defined($args{-Options}) ) {
        $args{-Options}->{"help|?"} = \$isHelp;
        $args{-Options}->{"man"}    = \$isMan;

        GetOptions(%{$args{-Options}}) or do {
            pod2usage(-exitval=>1, -verbose=>1, -output=>\*STDOUT, -input=>$args{-Prefs}->{_prefsfile_} );
            return undef;
        };

        if( $isHelp ) {
            print "\n", $args{-Prefs}->{_copyleft_}, "\n\n";
            pod2usage( -exitval=>0, -verbose=>1, -output=>\*STDOUT, -input=>$args{-Prefs}->{_prefsfile_} );
            return undef;
        }

        if( $isMan ) {
            print "\n", $args{-Prefs}->{_copyleft_}, "\n\n";
            pod2usage( -exitval=>0, -verbose=>2, -output=>\*STDOUT, -input=>$args{-Prefs}->{_prefsfile_} );
            return undef;
        }
    }

    $_theApp = $class->SUPER::new();
    $_theApp->_prefs($args{-Prefs} || {});
    $_theApp->_timer($startTime);
    $_theApp->_argv(\@_ARGV);
    return bless $_theApp, $class;
}

#sub prefs($)
#{
#    return $_[0]->_prefs;
#}
sub destroy
{}

sub pref
{
    my $self = shift or die;
    my $key = shift or die;
    my $value = shift;

    return $self->_prefs->{$key} unless defined $value;
    my $p = $self->_prefs;
    $p->{$key} = $value;
    return $self;
}

sub argv
{
    my $self = shift or die;
    return @{$self->_argv} if wantarray;
    return \@{$self->_argv};
}

sub timerReset($)
{
    $_[0]->_timer(time());
}

sub timerValue($)
{
    return time() - $_[0]->_timer;
}

sub handleException
{
    my $self = shift or die;
    die shift;
}

sub exec
{
    die "You must override the App::exec() function for your needs\n";
}
1;

package HgQuest::Parser;
use HgQuest::ClassLoader;
use cwt::Utils;
use JobQuest::Vacancy;
use JobQuest::Exception::DaysLimitExceeded;

use strict;

our @ISA = qw(JobQuest::ClassLoader);

use constant DEFAULT_DAYS => 7; # a week

sub new
{
    my $class = shift;
    my $sub_class = shift || die ('sub class name must be specified as a first argument');
    my %args = @_;
    my $reader = delete $args{'-reader'}; # || die ('-reader is a mandatory argument');
    my $writer = delete $args{'-writer'}; # || die ('-writer is a mandatory argument');
    my $dumper = delete $args{'-dumper'};
    my $id     = delete $args{'-id'} || '_id_unknown_';
    my $days   = delete $args{'-days'} || DEFAULT_DAYS;
    
    my $self = $class->SUPER::new($sub_class, %args);
    
    $self->{'reader'} = $reader;
    $self->{'writer'} = $writer;
    $self->{'vacancy'} = undef;
    $self->{'id'}     = $id;
    $self->{'days'}   = $days;
    return $self;
}


sub reader
{
    my $self = shift;
    my $reader = shift;
    return $self->{'reader'} unless defined($reader);
    $self->{'reader'} = $reader;
}

sub writer
{
    my $self = shift;
    my $writer = shift;
    return $self->{'writer'} unless defined($writer);
    $self->{'writer'} = $writer;
}

sub get
{
    my $self = shift;
    return $self->{'reader'}->get;
}


# saves a vacancy
sub save
{
    my $self = shift;
    
    die JobQuest::Exception::DaysLimitExceeded->new($self->{'days'})
        if jq_daysdiff($self->{'vacancy'}->get(JQC_DATE), 0+jq_today) > $self->{'days'};

    $self->{'vacancy'}->complete;	#generate JQID and other finalization action for the vacancy before writing
    $self->writer->write($self->{'vacancy'});
    $self->{'vacancy'} = undef;
}

1;

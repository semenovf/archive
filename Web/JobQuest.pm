package JQ::Web::JobQuest;
use strict;

use base qw(JQ::Web::Quest);


sub new {
    my $class = shift;
    my $self = bless $class->SUPER::new(@_), $class;
    $self->{'vlinks'} = {};
#    $self->{'vacancyHook'} = sub {};
    return $self;
}

=item $webQuest->newExpert($expert_name, $expert_class);

Create new expert instance of $expert_class and add it to 'experts` hash

=cut

sub newExpert($$$$)
{
    my ($self, $branch, $expert_name, $expert_class) = @_;
    my $expert = $self->SUPER::newExpert( $expert_name, $expert_class );
    $expert->branch($branch);
    return $expert;
}


#sub setVacancyHook
#{
#    my $self = shift or die;
#    my $arg = shift;
#
#    if( defined $arg ) {
#        if( ref $arg && ref $arg eq 'CODE' ) {
#            $self->{'vacancyHook'} = $arg;
#        }
#    }
#}
#

sub genVID($$)
{
    my ($self, $url) = @_;
    my $vid = $url;
    $vid =~ s/^.+:\/\///g;
    $vid =~ s/[^\w\d]/_/g;
    return $vid;
}

# add vacancy link
sub addVLink($$$)
{
    my ($self, $expert, $vlink) = @_;
    push @{$self->{'vlinks'}->{$expert->name}}, $vlink;
    return $self;
}

sub traverseVacancies($)
{
    my $self = $_[0];
    my $ua = $self->userAgent;

    foreach my $expert_name ( keys %{$self->{'vlinks'}} ) {
        my $expert = $self->{'experts'}->{$expert_name};

        foreach my $vlink ( @{$self->{'vlinks'}->{$expert_name}} ) {

            last if $expert->banned;
            last if $expert->inconsistent;

            if( $ua->isError ) {
                $self->setError($ua->lastError);
                $ua->clearErrors;
                last;
            }

            my $content = $ua->getContent($vlink);
            last unless $content;
            $expert->content($content);
            my $html_tree = $expert->buildHtmlTree;
            my $vac = $expert->parseVacancy($html_tree);
            if( $vac ) {
                $vac->{-Branch} = $expert->branch;
                $vac->{-Link} = $vlink;
                $vac->{-VID}  = $self->genVID($vlink);
                $expert->callback( -Vacancy=>$vac );
            }
            $html_tree->delete;
        }

        if( $expert->banned ) {
            $self->setError( '%s: expert banned', $expert->name );
        }

        # Expert must be updated (page structure may be changed)
        if( $expert->inconsistent ) {
            my $inc = $expert->inconsistent;
            $self->setError( '%s: node not found by %s at line %s', $expert->name, $inc->[0], $inc->[1] );
            $ua->cacheContent( $expert->content ) if $ua->cacheOnDemand;
        }
    }
}


sub exec($)
{
    $_[0]->SUPER::traverse();
    $_[0]->traverseVacancies();# unless $_[0]->isError;
}

1;

package JQ::Web::Quest;
use JQ::UserAgent;
use Module::Load;
use strict;

use base qw(JQ::Errorable);

sub new {
    my $class = shift;
    my $self = bless {
        'userAgent' => JQ::UserAgent->new,
        'experts'   => {}
    }, $class;
    return $self->makeErrorable;
}


sub foreach_expert
{
    my $self = shift or die;
    my $cb = shift;

    if( defined $cb && ref $cb && ref $cb eq 'CODE' ) {
        while( my ($expert_name, $expert ) = each( %{$self->{'experts'}} ) ) {
            $cb->( $expert );
        }
    }
}


sub userAgent($;)
{
    my $self = shift;
    my $ua = shift;
    return $self->{'userAgent'} unless defined $ua;
    return $self->{'userAgent'} = $ua;
}


sub expertExists($$)
{
    my ($self, $expert_name) = @_;
    return defined $self->{'experts'}->{$expert_name} ? 1 : 0;
}

=item $webQuest->newExpert($expert_name, $expert_class);

Create new expert instance of $expert_class and add it to 'experts` hash

=cut

sub newExpert($$$)
{
    my ($self, $expert_name, $expert_class) = @_;
    load $expert_class;
    my $expert = $expert_class->new();

    if( $expert ) {
        $self->addExpert($expert_name, $expert);
    }
    return $expert;
}


=item $webQuest->addExpert($expert_name, $expert);

Add expert

=cut

sub addExpert($$$)
{
    my ($self, $expert_name, $expert) = @_;
    if( defined $self->{'experts'}->{$expert_name} ) {
        $self->setError('expert with name \'%s\' is already added', $expert_name);
    } else {
        if( ref $expert && $expert->isa('JQ::Web::Quest::Expert') ) {
            $expert->quest($self);
            $expert->name($expert_name);
            $self->{'experts'}->{$expert_name} = $expert;
            #push @{$self->{'experts'}}, $expert;
        } else {
            $self->setError('trying to add something with name \'%s\' but it is not an expert (%s)',
                $expert_name,
                ref $expert ? 'REF to '.ref $expert : 'SCALAR');
        }
    }

    return $self;
}

=item $quest->traverse()

traverse pages

=cut

sub traverse($)
{
    my $self = $_[0];
    my $ua = $self->userAgent;

    foreach my $expert ( values %{$self->{'experts'}} ) {
        next if $expert->banned;
        next if $expert->inconsistent;

        my $it = $expert->begin();
        if( $it ) {
            LOOP: { do { # see 'Statement Modifiers' in perlsyn
                last LOOP if $expert->banned;
                last LOOP if $expert->inconsistent;

                if( $ua->isError ) {
                    $self->setError($ua->lastError);
                    $ua->clearErrors;
                    last LOOP;
                }

                $expert->parsePage( $it->asHtmlTree ) or last LOOP;

            } while( $it->next ); } # end of LOOP
            $it->delete;
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

        #print ref $expert, "\n";
    }
}

sub exec($)
{
    $_[0]->traverse();
}

1;

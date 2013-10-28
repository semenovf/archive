package JQ::UserAgent::Plan;
use strict;
use warnings;

use base 'Class::Accessor';

__PACKAGE__->mk_accessors(qw(
    userAgent
    nextPlan
));

sub new($)
{
    return bless $_[0]->SUPER::new(), __PACKAGE__;
}


sub getContent($$)
{
    $_[0]->nextPlan->getContent($_[1]);
}

sub addPlan($$)
{
    my ($self, $plan) = @_;
    my $p = $self->nextPlan;
    while( ref $p->nextPlan ne 'JQ::UserAgent::Plan::Final' ) {
        $p = $p->nextPlan;
    }
    $plan->nextPlan( $p->nextPlan );
    $p->nextPlan($plan);
}

package JQ::UserAgent::Plan::Final;
use base 'JQ::UserAgent::Plan';

sub new($)
{
    return bless $_[0]->SUPER::new(), __PACKAGE__;
}

sub getContent($$)
{
    my ($self, $url) = @_;
    $self->userAgent->_getContent($url);
}

package JQ::UserAgent::Plan::Root;
use base 'JQ::UserAgent::Plan';

sub new($)
{
    my $self = bless $_[0]->SUPER::new(), __PACKAGE__;
    $self->nextPlan(JQ::UserAgent::Plan::Final->new);
    return $self;
}

1;

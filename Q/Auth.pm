package JQ::Auth;
use Params::Validate;
use base 'Class::Accessor::Chained::Fast';
use strict;

JQ::Auth->mk_accessors(qw(user));

sub new
{
    my $class = shift;
    my $self = $class->SUPER::new(@_);
    return bless $self, $class;
}


sub isRole
{
    my $self = shift or die;
    my $role = shift or die;
}

#sub deploy
#{
#
#}


#sub isRegistered
#{
#
#}
#
#sub isAdmin
#{
#
#}

1;

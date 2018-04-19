package JQ::RC;
use JQ::Logger;
use strict;
use warnings;

use base qw(Exporter Class::Accessor);
our @EXPORT = qw(rc_fail rc_ok rc_usage);
#our @EXPORT_OK = qw(rc_usage);

my $WARN_ON_FAIL = 0;

__PACKAGE__->mk_accessors(qw(
    _ok
    _msg
    _rc
));

sub isOk
{
    my $self = $_[0];
    return $self->_ok ? 1 : 0;
}

sub errstr
{
    my $self = $_[0];
    return $self->_ok ? '' : $self->_msg;
}

sub rc
{
    my $self = $_[0];
    return $self->_ok ? $self->_rc : undef;
}

sub rc_fail($)
{
    my $self = __PACKAGE__->SUPER::new;
    $self->_ok(0);
    $self->_msg($_[0]);

    log_error $_[0] if $WARN_ON_FAIL;
    return $self;
}

sub rc_ok($)
{
    my $self = __PACKAGE__->SUPER::new;
    $self->_ok(1);
    $self->_rc($_[0]);
    return $self;
}

sub rc_usage($$)
{
    my $rc = $_[0];
    if( ref $rc ) {
        $$rc->{'usage'} = $_[1];
        return $$rc;
    }

    $rc->{'usage'} = $_[1];
    return $rc;
}

1;

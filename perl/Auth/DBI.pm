package JQ::Auth::DBI;
use Params::Validate;
use JQ::DBI qw(:ddi);
use JQ::DDI::Const;
use JQ::Exception;
use base 'JQ::Auth';
use strict;

my $ddiAuthDBI = {
    -DDI     => 'JQ::DDI::MySQL',
    -NS      => undef,
    -CHARSET => 'utf8',
    -CLASS   => {
        'jq_auth_roles' => { # roles, e.g. Admin|Superuser, Owner|Author, Editor, Viewer, Guest|Anonymous etc.
            'id'                => DDI_PK(0, 100),
            'name'              => DDI_TEXT(64),
            'desc'              => DDI_TEXT
        },
        'jq_auth_perms' => { # permissions
            'id'                => DDI_PK(0, 100),
            'name'              => DDI_TEXT(64),
            'desc'              => DDI_TEXT
        },
        'jq_auth_roles_perms' => { #
            'id'                => DDI_PK(0, 100*100),
            'role_id'           => DDI_REF('jq_auth_roles'),
            'perl_id'           => DDI_REF('jq_auth_perms')
        },
        'jq_auth_users' => {
            'id'                => DDI_PK(0, DDI_MAX_INT),
            'role_id'           => DDI_REF('jq_auth_roles'),
            'login'             => DDI_TEXT(32),
            'first_name'        => DDI_TEXT(64),
            'last_name'         => DDI_TEXT(64)
        }
    }
};



#BEGIN {
    Params::Validate::validation_options(
        on_fail=>sub { JQ::Exception::InvalidValue->throw($_[0]) }
    );
#}

#__PACKAGE__->mk_accessors(qw(dbh ns));

=head2 Usage:
    JQ::Auth::DBI->new()
=cut
sub new
{
    my $class = shift;
    my %params = @_;

    #JQ::Exception::InvalidValue->throw( sprintf('database handler for \'%s\' must be specified'), __PACKAGE__)
    #    unless($params{-dbh});

    #JQ::Exception::InvalidValue->throw( sprintf('namespace for \'%s\' must be specified'), __PACKAGE__)
    #    unless($params{-ddi_ns});

    #if( $params{-ddi_class} ) {
    #    $ddiAuthDBI->{-DDI} = $params{-ddi};
    #    delete $params{-ddi_class};
    #}
    #$self->dbh($params{-dbh});
    #$self->ns($params{-ddi_ns});

    my $self = $class->SUPER::new(%params);
    return bless $self, $class;
}

sub ddi { $ddiAuthDBI }

sub addRole {}
sub addUser
{

}
sub addPerm
{
    my ($self, %p) = validate_pos( @_,
        { type=>OBJECT },
        validate( @_, {
            -name=>{ type=>SCALAR },
            -desc=>{ type=>SCALAR }
        } ) );

    print 'permission added: ', $p{-name}, ' "', $p{-desc}, '" ', "\n";
}

sub deleteRole {}
sub deleteUser {}
sub deletePerm {}

sub isRole {}

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

__END__

=head1 Authentication and authorization

=cut

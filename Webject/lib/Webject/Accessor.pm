package Webject::Accessor;
use Carp;
use base Class::Accessor;


sub make_accessor {
    my ($class, $field) = @_;

    return sub {
        my $self = shift;

        if(@_) {
            return $self->set($field, @_);
        } else {
            return $self->get($field);
        }
    };
}


sub mk_accessors_with_validators
{
    my $self = shift;
    my %args = @_;
    
    $self->mk_accessors( keys %args );
    
    while ( my ($k, $v) = each %args ) {
        $self->{'-validators'}->{$k} = $v;
    }
}

sub set {
    my ($self, $key) = splice (@_, 0, 2);
    my $validator = $self->{'-validators'} ? $self->{'-validators'}->{$key} : undef;
    
    if( @_ == 1 ) {
        if( $validator ) {
            croak sprintf(q(Invalid value for '%s'), $key) unless $validator->is_valid( $_[0]);
        }
    } elsif( @_ > 1 ) {
        if( $validator ) {
            local $_;
            foreach( @_ ) {
                croak sprintf(q(Invalid value for '%s'), $_) unless $validator->is_valid( $_);
            }
        }        
    }
    
    $self->SUPER::set($key, @_);
    return $self;
}

1;
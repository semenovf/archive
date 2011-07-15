package MetaPage::Var;

sub new
{
    my $class = shift;
    return bless {}, $class;
}

sub get { return $_[0]->{$_[1]}; }
sub exists_var { return exists $_[0]->{$_[1]} ? 1 : 0; }

sub set
{
    my ($self, $name, $val) = @_;
    $self->{$name} = $val;
    return $self;
}

#
# set bulk of vars
#
# $vars->set_vars(\%hash)
# $vars->set_vars(name0 => val0, name1 => val1, ...)
#
sub set_vars
{
    my $self = shift;
    
    return $self unless @_ > 0;
    
    my $vars;
    if( @_ > 2 ) {
        my %vars = @_ if @_ > 2;
        $vars = \%vars;
    } else {
        $vars = $_[0];
    }
    
    while( my ($name,$val) = each %$vars ) {
        $self->{$name} = $val;
    }
    return $self;
}

1;

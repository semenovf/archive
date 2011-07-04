package Webject::RadioButton;
use base 'Webject';
use strict;

my @rw_accessors = qw(
    value
    checked
);

__PACKAGE__->mk_accessors(@rw_accessors);


sub new
{
    my $class = shift;
    my $self = $class->SUPER::new(@_);
    $self->caption('ok');
    $self->type(SUBMIT);
    return bless $self, $class;
}




package JQ::Webject::RadioButton;
use Params::Validate;
use JQ::WebApp;
use base 'JQ::Webject::Input';
use strict;


__PACKAGE__->mk_accessors(qw(value checked));

sub new {
    my ($class, $name, $value) = validate_pos(@_, 1, 1, {default=>''});
    my $self = $class->SUPER::new(@_);
    $self->name($name);
    $self->value($value);

    if( $value ) {
        my @values = JQ::WebApp->instance()->param($name);
        if( @values ) {
            $self->checked(1) if grep($_ eq $value, @values);
        }
    }
    return $self;
}





sub render
{
    my $self = shift or die;

    my $attrs = $self->attrs;
    my $html = '<input type="radio"';
    $html .= ' ' . $attrs if $attrs;
    $html .= ' value="' . $self->value . '"';
    $html .= ' checked="1"' if $self->checked;
    $html .= ' />';
    return $html;
}


1;

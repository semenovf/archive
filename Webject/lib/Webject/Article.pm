package Webject::Article;
use Validator::Var;
use Validator::Checker::Id qw(Id);
#use Validator::Checker::MostWanted qw(Length Base);
use base 'Webject';
use strict;
use warnings;

__PACKAGE__->mk_accessors(qw());

__PACKAGE__->mk_accessors_with_validators(
        descriptor  => Validator::Var->new()->checker(Id)->checker(Length, 32),
        description => Validator::Var->new()->checker(Length, 256),
        from        => Validator::Var->new()->checker([sub { $_[0] =~ /^\d+$/ }, '', 'is number']),
        rows        => Validator::Var->new()->checker([sub { $_[0] =~ /^\d+$/ }, '', 'is number']),
        limit       => Validator::Var->new()->checker([sub { $_[0] =~ /^\d+$/ }, '', 'is number'])
    );

sub ctor
{
    my $self = shift;
    $self->SUPER::ctor();
    $self->set_attributes( qw(name type value maxlength) );
    
    #$self->type(REGULAR);
    #$self->rows(1);
}

1;

__DATA__

<!--html{
<% if( $self->rows > 1 ) { %>
    <textarea <%=$self->render_atts%> rows=<%=$self->rows%>>
        <%=$self->render_children%>
    </textarea>
<% } else { %>
    <input <%=$self->render_atts%> />
<% } %>
}html-->
